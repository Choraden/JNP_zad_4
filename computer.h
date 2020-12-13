#ifndef COMPUTER_H
#define COMPUTER_H

using Id_t = uint64_t;

constexpr bool is_digit_or_letter(const char c) {
    return ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z') ||
           ('0' <= c && c <= '9');
}

constexpr uint64_t convert_char(const char c) {
    if ('A' <= c && c <= 'Z')
        c = c - 'A' + 'a';
    return c;
}

constexpr bool check_id(const char* str) {
    for (size_t i = 0; i < 7; i++) {
        if (str[i] == '\0')
            return i > 0;

        if (!is_digit_or_letter(str[i]))
            return false;
    }

    return true;
}

constexpr Id_t Id(const char* str) {
    uint64_t hash_const = 79;
    uint64_t res = 0;
    static_assert(check_id(str) == true, "Incorrect Id");

    for (size_t i = 0; i < 6; i++) {
        if (str[i] == '\0')
            break;

        res = res * hash_const + convert_char(str[i]);
    }

    return res;
}

typename <size_t mem_size, typename value_t>
constexpr void set_arth_flags (std::array<uint64_t, mem_size>& helper, value_t res) {
    helper[mem_size] = ((res == 0) ? 1 : 0); /// Pod indeksem mem_size jest flaga ZF.
    helper[mem_size + 1] = ((res < 0) ? 1 : 0); /// Pod indeksem mem_size + 1 jest flaga SF.
}

typename <size_t mem_size, typename value_t>
constexpr void set_log_flags (std::array<uint64_t, mem_size>& helper, value_t res) {
    helper[mem_size] = ((res == 0) ? 1 : 0); /// Pod indeksem mem_size jest flaga ZF.
}

template <typename Arg1, typename Arg2>
struct D {};

template <Id_t id>
struct Lea {};

template <typename T>
struct Mem {};

template <typename T>
struct Num {};

template <typename T>
struct Inc {};

template <typename T>
struct Dec {};

template <typename T>
struct Not {};

template <typename Arg1, typename Arg2>
struct Add {};

template <typename Arg1, typename Arg2>
struct Sub {};

template <typename Arg1, typename Arg2>
struct And {};

template <typename Arg1, typename Arg2>
struct Or {};

template <typename Arg1, typename Arg2>
struct Mov {};

template <typename Arg1, typename Arg2>
struct Cmp {};

template <Id_t id>
struct Label {};

template <Id_t id>
struct Jmp {};

template <Id_t id>
struct Jz {};

template <Id_t id>
struct Js {};

template <typename value_t, size_t mem_size, typename... Rest>
class Executors {
    template <typename operation>
    struct executor {};

    template <typename Arg>
    struct executor <Mem<Arg>> {
        static constexpr value_t write(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper, value_t new_val) {
            constexpr value_t id = executor<Arg>::value(mem, helper);
            static_assert(id >= 0 || id < mem_size, "Out of borders");
            mem[id] = new_val;
        }

        static constexpr value_t value(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper) {
            constexpr value_t id = executor<Arg>::value(mem, helper);
            static_assert(id >= 0 || id < mem_size, "Out of borders");
            return mem[id];
        }
    };

    template <value_t val>
    struct executor <Num<val>> {
        static constexpr value_t value(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper) {
            return val;
        }
    };

    template <Id_t id>
    struct executor<Lea<Id>> {
        static constexpr value_t value(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper) {
            for (i = 0; i < mem_size; i++) {
                if (id == helper[i])
                    return i;
            }
        }
    };

    template <typename Arg1, typename Arg2, typename... Tail>
    struct executor<Add<Arg1, Arg2>, Tail...> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper,
            bool protection, Id_t wanted_id, size_t last, size_t it) {

            if (protection == false) {
                executor<Arg1>::write(mem, helper, executor<Arg1>::value(mem, helper) + executor<Arg2>::value(mem, helper));
                value_t res = executor<Arg1>::value(mem, helper);
                set_arth_flags(helper, res);
            }
            else {
                static_assert(Tail.size() != 0, "Jump to nowhere.");
            }

            executor<Tail...>::execute(mem, helper, protection, wanted_id, last, it + 1);
        }
    };

    template <typename Arg1, typename Arg2, typename... Tail>
    struct executor<Sub<Arg1, Arg2>, Tail...> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper,
            bool protection, Id_t wanted_id, size_t last, size_t it) {

            if (protection == false) {
                executor<Arg1>::write(mem, helper, executor<Arg1>::value(mem, helper) - executor<Arg2>::value(mem, helper));
                value_t res = executor<Arg1>::value(mem, helper);
                set_arth_flags(helper, res);
            else {
                static_assert(Tail.size() != 0, "Jump to nowhere.");
            }

            executor<Tail...>::execute(mem, helper, protection, wanted_id, last, it + 1);
        }
    };

    template <typename Arg1, typename Arg2, typename... Tail>
    struct executor<And<Arg1, Arg2>, Tail...> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper,
            bool protection, Id_t wanted_id, size_t last, size_t it) {

            if (protection == false) {
                executor<Arg1>::write(mem, helper, executor<Arg1>::value(mem, helper) & executor<Arg2>::value(mem, helper));
                value_t res = executor<Arg1>::value(mem, helper);
                set_log_flags(helper, res);
            else {
                static_assert(Tail.size() != 0, "Jump to nowhere.");
            }

            executor<Tail...>::execute(mem, helper, protection, wanted_id, last, it + 1);
        }
    };

    template <typename Arg1, typename Arg2, typename... Tail>
    struct executor<Or<Arg1, Arg2>, Tail...> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper,
            bool protection, Id_t wanted_id, size_t last, size_t it) {

            if (protection == false) {
                executor<Arg1>::write(mem, helper, executor<Arg1>::value(mem, helper) | executor<Arg2>::value(mem, helper));
                value_t res = executor<Arg1>::value(mem, helper);
                set_log_flags(helper, res);
            else {
                static_assert(Tail.size() != 0, "Jump to nowhere.");
            }

            executor<Tail...>::execute(mem, helper, protection, wanted_id, last, it + 1);
        }
    };

    template <typename Arg1, typename Arg2, typename... Tail>
    struct executor<Mov<Arg1, Arg2>, Tail...> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper,
            bool protection, Id_t wanted_id, size_t last, size_t it) {

            if (protection == false) {
                executor<Arg1>::write(mem, helper, executor<Arg2>::value(mem, helper));
            else {
                static_assert(Tail.size() != 0, "Jump to nowhere.");
            }

            executor<Tail...>::execute(mem, helper, protection, wanted_id, last, it + 1);
        }
    };

    template <typename Arg1, typename Arg2, typename... Tail>
    struct executor<Cmp<Arg1, Arg2>, Tail...> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper,
            bool protection, Id_t wanted_id, size_t last, size_t it) {

            if (protection == false) {
                value_t res = executor<Arg1>::value(mem, helper) - executor<Arg2>::value(mem, helper);
                set_arth_flags(helper, res);
            else {
                static_assert(Tail.size() != 0, "Jump to nowhere.");
            }

            executor<Tail...>::execute(mem, helper, protection, wanted_id, last, it + 1);
        }
    };

    template <typename Arg, typename... Tail>
    struct executor<Not<Arg>, Tail...> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper,
            bool protection, Id_t wanted_id, size_t last, size_t it) {

            if (protection == false) {
                executor<Arg>::write(mem, helper, !(executor<Arg>::value(mem, helper)));
                value_t res = executor<Arg1>::value(mem, helper);
                set_log_flags(helper, res);
            else {
                static_assert(Tail.size() != 0, "Jump to nowhere.");
            }

            executor<Tail...>::execute(mem, helper, protection, wanted_id, last, it + 1);
        }
    };

    template <typename Arg, typename... Tail>
    struct executor<Inc<Arg>, Tail...> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper,
            bool protection, Id_t wanted_id, size_t last, size_t it) {

            if (protection == false) {
                executor<Arg1>::write(mem, helper, executor<Arg>::value(mem, helper) + 1);
                value_t res = executor<Arg1>::value(mem, helper);
                set_arth_flags(helper, res);
            else {
                static_assert(Tail.size() != 0, "Jump to nowhere.");
            }

            executor<Tail...>::execute(mem, helper, protection, wanted_id, last, it + 1);
        }
    };

    template <typename Arg, typename... Tail>
    struct executor<Dec<Arg>, Tail...> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper,
            bool protection, Id_t wanted_id, size_t last, size_t it) {

            if (protection == false) {
                executor<Arg1>::write(mem, helper, executor<Arg>::value(mem, helper) - 1);
                value_t res = executor<Arg1>::value(mem, helper);
                set_arth_flags(helper, res);
            else {
                static_assert(Tail.size() != 0, "Jump to nowhere.");
            }

            executor<Tail...>::execute(mem, helper, protection, wanted_id, last, it + 1);
        }
    };

    template<size_t it, Id_t id, typename N, typename... Tail>
    struct Declarations <it, D<id, N>, Tail...> {};

    template <size_t it, Id_t id, value_t val, typename... Tail>
    struct Declarations <it, D<id, Num<val>>, Tail...> {
        static constexpr void declare(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper) {
            static_assert(it < mem_size, "Out of borders");
            mem[it] = executor<Arg>::value(mem, helper);
            helper[it] = id;
            Declarations <it + 1, Tail...>::declare(mem, helper);
        }
    };

    template<Id_t id, typename... Tail>
    struct executor<Jump<id>, Tail...> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper,
            bool protection, Id_t wanted_id, size_t last, size_t it) {

            if (protection == false) {
                executor<Rest...>::execute(mem, helper, true, id, it, 0);
            }
            else {
                executor<Tail...>::execute(mem, helper, protection, wanted_id, last, it + 1);
            }
        }
    };

    template<Id_t id, typename... Tail>
    struct executor<Jz<id>, Tail...> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper,
            bool protection, Id_t wanted_id, size_t last, size_t it) {

            if (helper[mem_size] == 0) {
                executor<Tail...>::execute(mem, helper, protection, wanted_id, last, it + 1);
            }
            else {
                if (protection == false) {
                    executor<Rest...>::execute(mem, helper, true, id, it, 0);
                }
                else {
                    executor<Tail...>::execute(mem, helper, protection, wanted_id, last, it + 1);
                }
            }
        }
    };

    template<Id_t id, typename... Tail>
    struct executor<Js<id>, Tail...> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper,
            bool protection, Id_t wanted_id, size_t last, size_t it) {

            if (helper[mem_size + 1] == 0) {
                executor<Tail...>::execute(mem, helper, protection, wanted_id, last, it + 1);
            }
            else {
                if (protection == false) {
                    executor<Rest...>::execute(mem, helper, true, id, it, 0);
                }
                else {
                    executor<Tail...>::execute(mem, helper, protection, wanted_id, last, it + 1);
                }
            }
        }
    };

    template<Id_t id, typename... Tail>
    struct executor<Label<id>, Tail...> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper,
            bool protection, Id_t wanted_id, size_t last, size_t it) {

            if (protection == false || (protection == true && id != wanted_id)) {
                executor<Tail...>::execute(mem, helper, protection, wanted_id, last, it + 1);
            }
            else {
                static_assert(it > last, "Infinite jumping.");
                executor<Tail...>::execute(mem, helper, false, wanted_id, last, it + 1);
            }
        }
    };

    template<Id_t id, typename N, typename... Tail>
    struct executor<D<id, N>, Tail...> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper,
            bool protection, Id_t wanted_id, size_t last, size_t it) {

            executor<Tail...>::execute(mem, helper, protection, wanted_id, last, it + 1);
        }
    };

public:
    static constexpr mem execution() {
        size_t beginning = 0;
        Declarations<beginning, Rest...>::declare(mem, helper);
        Id_t id_wanted = 0;
        size_t last = 0;
        size_t it = 0;
        executor<Rest...>::execute(mem, helper, false, id_wanted, last, it);

        return mem;
    }
};

template <size_t mem_size, typename value_t>
class Computer {
    std:array<value_t, mem_size> mem;
    std::array<Id_t, mem_size + 2> helper;

    template <typename Instructions>
    struct Loading {};

    template <typename Instruction, typename ... Rest>
    struct Loading <Instruction, Rest ...> {
        static constexpr mem execution() {
            return Executors<value_t, mem_size, Rest...>::execution();
        }
    }

public:
    template <typename Instructions>
    static constexpr mem boot() {
        return Loading<Instructions>::execution();
    }  
};

#endif //COMPUTER_H
