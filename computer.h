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

struct Id {
    const uint64_t hash;
    constexpr explicit Id(const char* str) : hash(get_id_hash(str)) {}
};

template <Id& id>
struct Lea {
    static constexpr uint64_t value = id.hash;
};


template <size_t mem_size, typename value_t>
class Computer {
    std::array<value_t, mem_size> mem;

public:
    //TODO
};

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
        static constexpr value_t write(std::array<value_t, mem_size> &mem, value_t new_val) {
            constexpr value_t id = executor<Arg>::value(mem);
            static_assert(id >= 0 || id < mem_size, "Out of borders");
            mem[id] = new_val;
        }

        static constexpr value_t value(std::array<value_t, mem_size> &mem) {
            constexpr value_t id = executor<arg>::value(mem);
            static_assert(id >= 0 || id < mem_size, "Out of borders");
            return mem[id];
        }
    };

    template <value_t val>
    struct executor <Num<val>> {
        static constexpr value_t value(std::array<value_t, mem_size> &mem) {
            return val;
        }
    };

    template <typename Arg1, typename Arg2>
    struct executor<Add<Arg1, Arg2>> {
        static constexpr void execute(std::array<value_t, mem_size> &mem) {
            executor<Arg1>::write(mem, executor<Arg1>::value(mem) + executor<Arg2>::value(mem));
        }
    };

    template <typename Arg1, typename Arg2>
    struct executor<Sub<Arg1, Arg2>> {
        static constexpr void execute(std::array<value_t, mem_size> &mem) {
            executor<Arg1>::write(mem, executor<Arg1>::value(mem) - executor<Arg2>::value(mem));
        }
    };

    template <typename Arg1, typename Arg2>
    struct executor<And<Arg1, Arg2>> {
        static constexpr void execute(std::array<value_t, mem_size> &mem) {
            executor<Arg1>::write(mem, executor<Arg1>::value(mem) & executor<Arg2>::value(mem));
        }
    };

    template <typename Arg1, typename Arg2>
    struct executor<Or<Arg1, Arg2>> {
        static constexpr void execute(std::array<value_t, mem_size> &mem) {
            executor<Arg1>::write(mem, executor<Arg1>::value(mem) | executor<Arg2>::value(mem));
        }
    };

    template <typename Arg1, typename Arg2>
    struct executor<Mov<Arg1, Arg2>> {
        static constexpr void execute(std::array<value_t, mem_size> &mem) {
            executor<Arg1>::write(mem, executor<Arg2>::value(mem));
        }
    };

    template <typename Arg>
    struct executor <Not<Arg>> {
        static constexpr value_t execute(std::array<value_t, mem_size> &mem) {
            executor<Arg>::write(arr, !(executor<Arg>::value(mem)));
        }
    };

};



#endif //COMPUTER_H
