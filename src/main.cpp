#include <iostream>
#include "crystal.h"

int main() {
    auto lat = make_face_centered_cubic();
    Atom atom = {{0.0, 0.0, 0.0}, 1};
    Cell cell = {{atom}};
    Crystal cry(lat, cell);
    std::cout << cry; 
    return 0;
}
