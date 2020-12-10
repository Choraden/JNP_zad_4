#ifndef COMPUTER_H
#define COMPUTER_H

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

constexpr uint64_t get_id_hash(const char* str) {
    uint64_t hash_const = 79;
    uint64_t res = 0;
    if (check_id()) {
        for (size_t i = 0; i < 6; i++) {
            if (str[i] == '\0')
                break;

            res = res * hash_const + convert_char(std[i]);
        }
    }
    return res;
}

template <size_t mem_size, typename value_t>
class Computer {
    std:array<value_t, mem_size> arr;
    std::array<uint64_t, mem_size> mem;


public:
    //TODO
};

struct Id {
    const uint64_t hash;
    constexpr explicit Id(const char* str) : hash(get_id_hash(str)) {}
};

struct Label {
    const uint64_t
};

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

/// TODO flagi - nie wiem co i jak z nimi.
template <typename value_t, size_t mem_size>
class executors {
    template <typename operation>
    struct executor;

    template <typename Arg>
    struct executor <Mem<Arg>> {
        static constexpr value_t write(std::array<uint64_t, mem_size> &mem, std::array<value_t, mem_size> &arr, value_t new_val) {
            constexpr value_t id = executor<Arg>::value(mem, arr);
            static_assert(id >= 0 || id < mem_size, "Out of borders");
            arr[id] = new_val;
        }

        static constexpr value_t value(std::array<uint64_t, mem_size> &mem, std::array<value_t, mem_size> &arr) {
            constexpr value_t id = executor<arg>::value(mem, arr);
            static_assert(id >= 0 || id < mem_size, "Out of borders");
            return arr[id];
        }
    };

    template <value_t val>
    struct executor <Num<val>> {
        static constexpr value_t value(std::array<uint64_t, mem_size> &mem, std::array<value_t, mem_size> &arr) {
            return val;
        }
    };

    template<Id id>
    struct executor<Lea<id>> {
        static constexpr value_t value(std::array<uint64_t, mem_size> &mem, std::array<value_t, mem_size> &arr) {
            for (i = 0; i < mem_size; i++) {
                if (id.hash == mem[i])
                    return i;
            }
        }
    };

    template <typename Arg1, typename Arg2>
    struct executor<Add<Arg1, Arg2>> {
        static constexpr void execute(std::array<uint64_t, mem_size> &mem, std::array<value_t, mem_size> &arr) {
            executor<Arg1>::write(mem, arr, executor<Arg1>::value(mem, arr) + executor<Arg2>::value(mem, arr));
        }
    };

    template <typename Arg1, typename Arg2>
    struct executor<Sub<Arg1, Arg2>> {
        static constexpr void execute(std::array<uint64_t, mem_size> &mem, std::array<value_t, mem_size> &arr) {
            executor<Arg1>::write(mem, arr, executor<Arg1>::value(mem, arr) - executor<Arg2>::value(mem, arr));
        }
    };

    template <typename Arg1, typename Arg2>
    struct executor<And<Arg1, Arg2>> {
        static constexpr void execute(std::array<uint64_t, mem_size> &mem, std::array<value_t, mem_size> &arr) {
            executor<Arg1>::write(mem, arr, executor<Arg1>::value(mem, arr) & executor<Arg2>::value(mem, arr));
        }
    };

    template <typename Arg1, typename Arg2>
    struct executor<Or<Arg1, Arg2>> {
        static constexpr void execute(std::array<uint64_t, mem_size> &mem, std::array<value_t, mem_size> &arr) {
            executor<Arg1>::write(mem, arr, executor<Arg1>::value(mem, arr) | executor<Arg2>::value(mem, arr));
        }
    };

    template <typename Arg1, typename Arg2>
    struct executor<Mov<Arg1, Arg2>> {
        static constexpr void execute(std::array<uint64_t, mem_size> &mem, std::array<value_t, mem_size> &arr) {
            executor<Arg1>::write(mem, arr, executor<Arg2>::value(mem, arr));
        }
    };

    template <typename Arg1, typename Arg2>
    struct executor<Cmp<Arg1, Arg2>> {
        static constexpr void execute(std::array<uint64_t, mem_size> &mem, std::array<value_t, mem_size> &arr) {
            value_t res = executor<Arg1>::value(mem, arr) - executor<Arg2>::value(mem, arr);

        }
    };

    template <typename Arg>
    struct executor <Not<Arg>> {
        static constexpr value_t execute(std::array<uint64_t, mem_size> &mem, std::array<value_t, mem_size> &arr) {
            executor<Arg>::write(mem, arr, !(executor<Arg>::value(mem, arr)));
        }
    };

    template <typename Arg>
    struct executor<Inc<Arg>> {
        static constexpr void execute(std::array<uint64_t, mem_size> &mem, std::array<value_t, mem_size> &arr) {
            executor<Arg1>::write(mem, arr, executor<Arg>::value(mem, arr) + 1);
        }
    };

    template <typename Arg>
    struct executor<Dec<Arg>> {
        static constexpr void execute(std::array<uint64_t, mem_size> &mem, std::array<value_t, mem_size> &arr) {
            executor<Arg1>::write(mem, arr, executor<Arg>::value(mem, arr) - 1);
        }
    };
};

#endif //COMPUTER_H
