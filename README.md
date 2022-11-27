[![C/C++ CI Fedora 37](https://github.com/rofirrim/libadwaitamm/actions/workflows/meson-fedora.yml/badge.svg)](https://github.com/rofirrim/libadwaitamm/actions/workflows/meson-fedora.yml)

## About

libadwaitamm is a C++ binding for the libadwaita library.

## How to build

Make sure you fulfill the following dependences:

 - `>=meson-0.56`
 - `>=glibmm-2.73.2`
 - `>=pangomm-2.50.0`
 - `>=gtkmm-4.6.1`
 - `>=libadwaita-1.2.0`
 - `>=mm-common-get-1.0.0`
   - This is only needed when building in maintainer mode which is the default when not building from a tarball.

If you need a reference environment, Fedora 37 has all the required components
already packaged.

## Issues and comments

Known issues:
 - **The API is not stable yet. There are no immediate plans to stabilise it yet.**
 - We probably want to port libadwaita's tests.
 - The documentation may be a bit "iffy" as it has not been reviewed yet.

Feel free to raise issues to keep track of problems and improvements in the
wrapper.

Please, be mindful that this is free software that may be developed on
contributors' free time and as such it does not come with any sort of
guaranteed support of any kind. This development is made available in the hope
it can be useful to the community.
