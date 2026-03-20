/**============================================================================
Name        : main.cpp
Created on  : 20.03.2025
Author      : Tokmakov Andrei
Version     : 1.0
Copyright   : Your copyright notice
Description : Tests C++ project
============================================================================**/

#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <cassert>

namespace
{
    constexpr uint8_t charBit { 8 };
    constexpr uint16_t maxValue { sizeof(uint16_t) * charBit };

    [[nodiscard]]
    constexpr bool is_set(const uint16_t mask, const uint16_t index) noexcept {
        return index < maxValue && (mask & (1u << index));
    }

    [[nodiscard]]
    constexpr bool is_not_set(const uint16_t mask, const uint16_t index) noexcept {
        return !is_set(mask, index);
    }

    constexpr void unset_bit(uint16_t& mask, const uint16_t bit) noexcept {
        mask &= ~(1 << bit);
    };

    constexpr void set_bit(uint16_t& mask, const uint16_t bit) noexcept {
        if (bit < maxValue) {
            mask |= (1 << bit);
        }
    }
}


namespace
{
    struct Iterator
    {
        using index_type = uint16_t;
        using value_type = uint16_t;
        using mask_type  = uint16_t;

        constexpr Iterator(const mask_type msk, const index_type startIndex, const index_type endIndex):
            mask { msk }, idx { startIndex }, endIdx { endIndex }
        {
            while (idx < endIdx && isNotSet(idx)) {
                ++idx;
            }
        }

        constexpr Iterator& operator++() noexcept
        {
            do {
                ++idx;
            } while (idx < endIdx && isNotSet(idx));
            return *this;
        }

        constexpr value_type operator*() const noexcept {
            return idx;
        }

        [[nodiscard]]
        constexpr bool operator==(const Iterator& other) const noexcept {
            return idx == other.idx;
        }

        [[nodiscard]]
        constexpr bool operator!=(const Iterator& other) const noexcept {
            return !(*this == other);
        }

    private:
        /** **/
        mask_type  mask { 0 };

        /** **/
        index_type idx { 0 };

        /** Index of the element after the last one: --> { size + 1 } **/
        index_type endIdx { 0 };


        [[nodiscard]]
        constexpr bool isSet(const index_type index) const noexcept
        {
            return endIdx > index && is_set(mask, index);
        }

        [[nodiscard]]
        constexpr bool isNotSet(const index_type index) const noexcept
        {
            return !isSet(index);
        }
    };

    struct Set
    {
        using index_type = uint16_t;
        using size_type  = uint16_t;
        using value_type = uint16_t;

        constexpr Set() = default;

        void insert(const value_type val)
        {
            if (val >= maxValue) {
                return;
            }
            set_bit(mask, val);
            size = std::max(size, static_cast<size_type>(val + 1));            
        }

        constexpr void erase(const value_type val)
        {
            unset_bit(mask, val);
            while (size > 0 && is_not_set(mask, size - 1)) {
                --size;
            }
        }

        [[nodiscard]]
        constexpr bool contains(const value_type val) const noexcept {
            return is_set(mask, val);
        }

        [[nodiscard]]
        constexpr Iterator begin() const noexcept {
            return Iterator { mask, 0, size };
        }

        [[nodiscard]]
        constexpr Iterator end() const noexcept {
            return Iterator { mask, size, size };
        }

    private:

        value_type mask { 0 };
        size_type size { 0 };
    };

    [[maybe_unused]]
    std::ostream& operator<<(std::ostream& os, const Set& set)
    {
        for (const auto i : set)
            os << i << " ";
        return os ;
    }
}


void simpleTest()
{
    constexpr std::array<int, 3> data1 {2, 8, 12};
    constexpr std::array<int, 2> data2 {2, 12};

    Set set {};
    for (int v: data1) {
        set.insert(v);
    }
    for (int v: data1) {
        assert(set.contains(v));
    }
    for (uint32_t idx = 0; const auto i : set) {
        assert(data1[idx++] == i);
    }

    set.erase(8);

    for (int v: data2) {
        assert(set.contains(v));
    }
    for (uint32_t idx = 0; const auto i : set) {
        assert(data2[idx++] == i);
    }
}

int main(const int argc,
         char** argv,
         char** environment)
{
    const std::vector<std::string_view> args(argv + 1, argv + argc);
    const std::map<std::string_view, std::string_view> env = [&] {
        std::map<std::string_view, std::string_view> envs;
        for (int i = 0; environment && environment[i]; ++i) {
            const std::string_view envVar = environment[i];
            if (const size_t pos = envVar.find('='); std::string::npos != pos) {
                envs.emplace(envVar.substr(0, pos), envVar.substr(pos + 1));
            }
        }
        return envs;
    }();

    simpleTest();

    return EXIT_SUCCESS;
}

