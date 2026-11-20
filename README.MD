# BUILD

cmake > 3.23

1 step: 
  install conan2 and configure project
  a step:
    `conan profile detect --force`

  b step:
    `conan install . --build=missing`

2 step: 
  configure cmake with conan toolchain
  `cmake --preset conan-release`

3 step:
  build
  `cmake --build --preset conan-release`
