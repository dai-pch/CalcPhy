#ifndef CRYSTAL_H__
#define CRYSTAL_H__

#include <vector>
#include <memory>
#include <iostream>
#include "number/Mat.h"
#include "common.h"

struct LatPnt {
    vec3i reR;
    vec3d r;
};

struct Atom {
    using Cord = vec3d;
    Cord r;
    const int type;

    void move(const Cord& dr) {
        r = r + dr;
    }
    void moveTo(const Cord& dr) {
        r = dr;
    }
};

struct Cell {
    const std::vector<Atom> _atoms;
};

class Lattice {
public:
    using Cord = vec3d;
public:
    Lattice(const Cord a1, const Cord a2, const Cord a3,
        const int n1, const int n2, const int n3): _transVec(concat(a1, a2, a3)),
        _n1(n1), _n2(n2), _n3(n3), _lats(new std::vector<LatPnt>()) {
        construct_lattice();
    }

    auto begin() const {
        return _lats->begin();
    }
    auto end() const {
        return _lats->end();
    }

private:
    void construct_lattice() {
        _lats->reserve(_n1*_n2*_n3);
        for (int i1=0;i1<_n1;++i1) {
            for (int i2=0;i2<_n2;++i2) {
                for (int i3=0;i3<_n3;++i3) {
                    LatPnt t;
                    t.reR = {i1, i2, i3};
                    t.r = _transVec * t.reR;
                    _lats->push_back(t);
                }
            }
        }
    }

private:
    const Number::Mat<double, 3, 3> _transVec;
    const size_t _n1, _n2, _n3;
    const std::shared_ptr<std::vector<LatPnt>> _lats;
}; // class Lattice

class Crystal {
    using Cord = vec3d;
public:
    Crystal(const Lattice& lat, const Cell& cell): _lat(lat), _cell(cell), _atoms(new std::vector<Atom>()) {
        construct_crystal();
    }

    friend std::ostream& operator<<(std::ostream& os, const Crystal& cry) {
        os << cry._atoms->size() << '\n';
        os << "id type x y z\n";
        for (size_t ii = 0; ii < cry._atoms->size(); ++ii){
            auto& atom = (*cry._atoms)[ii];
            os << ii << ' ' << atom.type << atom.r[0] << ' ' << atom.r[1] << ' ' << atom.r[2] << '\n';
        }
        os << cry._atoms->size();
        os << std::endl;
        return os;
    }

private:
    void construct_crystal() {
        for (auto it = _lat.begin(); it != _lat.end(); ++it) {
            for (auto at = _cell._atoms.begin(); at != _cell._atoms.end(); ++at) {
                Atom t(*at);
                t.move(it->r);
                _atoms->push_back(t);
            }
        }
    }

private:
    Lattice _lat;
    Cell _cell;
    std::shared_ptr<std::vector<Atom>> _atoms;
}; // class Crystal

Lattice make_simple_cubic() {
    Lattice::Cord a1 = {1.0, 0.0, 0.0};
    Lattice::Cord a2 = {0.0, 1.0, 0.0};
    Lattice::Cord a3 = {0.0, 0.0, 1.0};
    size_t n1 = 5;
    size_t n2 = 5;
    size_t n3 = 5;
    return Lattice(a1, a2, a3, n1, n2, n3);
}

Lattice make_body_centered_cubic() {
    Lattice::Cord a1 = {0.5, 0.5, -0.5};
    Lattice::Cord a2 = {-0.5, 0.5, 0.5};
    Lattice::Cord a3 = {0.5, -0.5, 0.5};
    size_t n1 = 5;
    size_t n2 = 5;
    size_t n3 = 5;
    return Lattice(a1, a2, a3, n1, n2, n3);
}

Lattice make_face_centered_cubic() {
    Lattice::Cord a1 = {0.5, 0.5, 0.0};
    Lattice::Cord a2 = {0.0, 0.5, 0.5};
    Lattice::Cord a3 = {0.5, 0.0, 0.5};
    size_t n1 = 5;
    size_t n2 = 5;
    size_t n3 = 5;
    return Lattice(a1, a2, a3, n1, n2, n3);
}

#endif // CRYSTAL_H__