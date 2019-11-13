#include <utility>

namespace azgra
{
    template <typename T>
    class Optional
    {
    private:
        bool m_isValid = false;
        T m_value{};

    public:
        Optional() noexcept
        {
            m_isValid = false;
        }

        Optional(T &value) noexcept
        {
            m_isValid = true;
            m_value = std::move(value);
        }

        Optional(const T &value) noexcept
        {
            m_isValid = true;
            m_value = value;
        }

        // Optional(T value) noexcept
        // {
        //     m_isValid = true;
        //     m_value = value;
        // }

        T value() noexcept
        {
            return m_value;
        }

        T const& value() const noexcept
        {
            return m_value;
        }

        constexpr operator bool() const
        {
            return m_isValid;
        }

        constexpr bool is_valid() const noexcept
        {
            return m_isValid;
        }
    };
} // namespace azgra
