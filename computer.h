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
    return false;
}

constexpr Id_t Id(const char* str) {
    uint64_t hash_const = 79;
    uint64_t res = 0;
    if (check_id(str)) {
        for (size_t i = 0; i < 6; i++) {
            if (str[i] == '\0')
                break;

            res = res * hash_const + convert_char(str[i]);
        }
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

struct Label {};

template <Id id>
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


template <typename value_t, size_t mem_size>
class executors {
    template <typename operation>
    struct executor {};


    struct executor<D<Id_t id, value_t val>> {
        // TODO + nie wiem czy ten param^ jest ok.
    };

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

    template<Id_t Id>
    struct executor<Lea<Id>> {
        static constexpr value_t value(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper) {
            for (i = 0; i < mem_size; i++) {
                if (Id == helper[i])
                    return i;
            }
        }
    };

    template <typename Arg1, typename Arg2>
    struct executor<Add<Arg1, Arg2>> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper) {
            executor<Arg1>::write(mem, helper, executor<Arg1>::value(mem, helper) + executor<Arg2>::value(mem, helper));
            value_t res = executor<Arg1>::value(mem, helper);
            set_arth_flags(helper, res);
        }
    };

    template <typename Arg1, typename Arg2>
    struct executor<Sub<Arg1, Arg2>> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper) {
            executor<Arg1>::write(mem, helper, executor<Arg1>::value(mem, helper) - executor<Arg2>::value(mem, helper));
            value_t res = executor<Arg1>::value(mem, helper);
            set_arth_flags(helper, res);
        }
    };

    template <typename Arg1, typename Arg2>
    struct executor<And<Arg1, Arg2>> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper) {
            executor<Arg1>::write(mem, helper, executor<Arg1>::value(mem, helper) & executor<Arg2>::value(mem, helper));
            value_t res = executor<Arg1>::value(mem, helper);
            set_log_flags(helper, res);
        }
    };

    template <typename Arg1, typename Arg2>
    struct executor<Or<Arg1, Arg2>> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper) {
            executor<Arg1>::write(mem, helper, executor<Arg1>::value(mem, helper) | executor<Arg2>::value(mem, helper));
            value_t res = executor<Arg1>::value(mem, helper);
            set_log_flags(helper, res);
        }
    };

    template <typename Arg1, typename Arg2>
    struct executor<Mov<Arg1, Arg2>> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper) {
            executor<Arg1>::write(mem, helper, executor<Arg2>::value(mem, helper));
        }
    };

    template <typename Arg1, typename Arg2>
    struct executor<Cmp<Arg1, Arg2>> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper) {
            value_t res = executor<Arg1>::value(mem, helper) - executor<Arg2>::value(mem, helper);
            set_arth_flags(helper, res);
        }
    };

    template <typename Arg>
    struct executor <Not<Arg>> {
        static constexpr value_t execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper) {
            executor<Arg>::write(mem, helper, !(executor<Arg>::value(mem, helper)));
            value_t res = executor<Arg1>::value(mem, helper);
            set_log_flags(helper, res);
        }
    };

    template <typename Arg>
    struct executor<Inc<Arg>> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper) {
            executor<Arg1>::write(mem, helper, executor<Arg>::value(mem, helper) + 1);
            value_t res = executor<Arg1>::value(mem, helper);
            set_arth_flags(helper, res);
        }
    };

    template <typename Arg>
    struct executor<Dec<Arg>> {
        static constexpr void execute(std::array<value_t, mem_size> &mem, std::array<uint64_t, mem_size> &helper) {
            executor<Arg1>::write(mem, helper, executor<Arg>::value(mem, helper) - 1);
            value_t res = executor<Arg1>::value(mem, helper);
            set_arth_flags(helper, res);
        }
    };
};

template <size_t mem_size, typename value_t>
class Computer {
    std:array<value_t, mem_size> mem;
    std::array<uint64_t, mem_size + 2> helper;

public:
    //TODO
};

#endif //COMPUTER_H
