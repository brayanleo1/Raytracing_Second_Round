#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <initializer_list>

/// Math structures namespace.
namespace ms
{
    /// Implements the structure of a 3 position vector.
    class vec3
    {
    public:
        vec3() {}
        vec3(double e0, double e1, double e2)
        {
            e[0] = e0;
            e[1] = e1;
            e[2] = e2;
            m_end = 3;
        }
        vec3(std::initializer_list<double> values)
        {
            // Verifica se a lista de inicialização tem 3 elementos
            if (values.size() != 3)
            {
                // Lança uma exceção ou imprime uma mensagem de erro, conforme necessário
                throw std::runtime_error("Error: initialization list must have three elements");
            }

            // Inicializa os valores de x, y e z a partir da lista de inicialização
            auto it = values.begin();
            e[0] = *it++;
            e[1] = *it++;
            e[2] = *it;
            m_end = 3;
        }
        inline double x() const { return e[0]; }
        inline double y() const { return e[1]; }
        inline double z() const { return e[2]; }
        inline double r() const { return e[0]; }
        inline double g() const { return e[1]; }
        inline double b() const { return e[2]; }

        int m_end = 0;

        inline const vec3 &operator+() const { return *this; }
        inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        inline double operator[](int i) const { return e[i]; }
        inline double &operator[](int i) { return e[i]; };

        inline vec3 &operator+=(const vec3 &v2);
        inline vec3 &operator-=(const vec3 &v2);
        inline vec3 &operator*=(const vec3 &v2);
        inline vec3 &operator/=(const vec3 &v2);
        inline vec3 &operator*=(const double t);
        inline vec3 &operator/=(const double t);

        inline double length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
        inline double size() const { return m_end; }
        inline double squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
        inline void make_unit_vector();

        double e[3];
    };

    inline std::istream &operator>>(std::istream &is, vec3 &t)
    {
        is >> t.e[0] >> t.e[1] >> t.e[2];
        return is;
    }

    inline std::ostream &operator<<(std::ostream &os, const vec3 &t)
    {
        os << t.e[0] << " " << t.e[1] << " " << t.e[2];
        return os;
    }

    inline void vec3::make_unit_vector()
    {
        double k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
        e[0] *= k;
        e[1] *= k;
        e[2] *= k;
    }

    inline vec3 operator+(const vec3 &v1, const vec3 &v2)
    {
        return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
    }

    inline vec3 operator-(const vec3 &v1, const vec3 &v2)
    {
        return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
    }

    inline vec3 operator*(const vec3 &v1, const vec3 &v2)
    {
        return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
    }

    inline vec3 operator/(const vec3 &v1, const vec3 &v2)
    {
        return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
    }

    inline vec3 operator*(double t, const vec3 &v)
    {
        return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
    }

    inline vec3 operator/(vec3 v, double t)
    {
        return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
    }

    inline vec3 operator*(const vec3 &v, double t)
    {
        return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
    }

    inline double dot(const vec3 &v1, const vec3 &v2)
    {
        return v1.e[0] * v2.e[0]
             + v1.e[1] * v2.e[1]
             + v1.e[2] * v2.e[2];
    }

    inline vec3 cross(const vec3 &v1, const vec3 &v2)
    {
        return vec3((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
                    (v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2]),
                    (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
    }

    inline vec3 &vec3::operator+=(const vec3 &v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    inline vec3 &vec3::operator*=(const vec3 &v)
    {
        e[0] *= v.e[0];
        e[1] *= v.e[1];
        e[2] *= v.e[2];
        return *this;
    }

    inline vec3 &vec3::operator/=(const vec3 &v)
    {
        e[0] /= v.e[0];
        e[1] /= v.e[1];
        e[2] /= v.e[2];
        return *this;
    }

    inline vec3 &vec3::operator-=(const vec3 &v)
    {
        e[0] -= v.e[0];
        e[1] -= v.e[1];
        e[2] -= v.e[2];
        return *this;
    }

    inline vec3 &vec3::operator*=(const double t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    inline vec3 &vec3::operator/=(const double t)
    {
        double k = 1.0 / t;

        e[0] *= k;
        e[1] *= k;
        e[2] *= k;
        return *this;
    }

    inline vec3 unit_vector(vec3 v)
    {
        return v / v.length();
    }

    /// Implements the structure of a 2 position vector.
    class vec2 {
public:
    vec2() {}
    vec2(double e0, double e1) : e{ e0, e1 } {}
    vec2(std::initializer_list<double> values)
    {
        // Verifica se a lista de inicialização tem 2 elementos
        if (values.size() != 2) {
            // Lança uma exceção ou imprime uma mensagem de erro, conforme necessário
            throw std::runtime_error("Error: initialization list must have two elements");
        }

        // Inicializa os valores de x e y a partir da lista de inicialização
        auto it = values.begin();
        e[0] = *it++;
        e[1] = *it++;
    }

    /*vec2(const vec2& other) {
        e[0] = other.e[0];
        e[1] = other.e[1];
        m_end = other.m_end;
    }*/

    // Copy constructor
    vec2(const vec2& other) {
        e[0] = other.e[0];
        e[1] = other.e[1];
        m_end = other.m_end;
    }

    // Copy assignment operator
    vec2& operator=(const vec2& other) {
        e[0] = other.e[0];
        e[1] = other.e[1];
        m_end = other.m_end;
        return *this;
    }

    /*vec2& operator=(const vec2& other) {
        if (this != &other) {
            e[0] = other.e[0];
            e[1] = other.e[1];
            m_end = other.m_end;
        }
        return *this;
    }*/

    inline double x() const { return e[0]; }
    inline double y() const { return e[1]; }
    inline double r() const { return e[0]; }
    inline double g() const { return e[1]; }

    int m_end = 0;

    inline const vec2& operator+() const { return *this; }
    inline vec2 operator-() const { return vec2(-e[0], -e[1]); }
    inline double operator[](int i) const { return e[i]; }
    inline double& operator[](int i) { return e[i]; };

    inline vec2& operator+=(const vec2& v2);
    inline vec2& operator-=(const vec2& v2);
    inline vec2& operator*=(const vec2& v2);
    inline vec2& operator/=(const vec2& v2);
    inline vec2& operator*=(const double t);
    inline vec2& operator/=(const double t);

    inline double length() const { return sqrt(e[0] * e[0] + e[1] * e[1]); }
    inline double size() const { return m_end; }
    inline double squared_length() const { return e[0] * e[0] + e[1] * e[1]; }
    inline void make_unit_vector();

    double e[2];
};
}