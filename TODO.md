# TODO

Issues, extensions and potential improvements:

- Add [Doxygen](https://www.doxygen.org) support to the build for the `blinkt_functions` documentation.
- Investigate alternatives to `wiringpi` now that it has been deprecated. Possibilities include [pigpio](http://abyz.me.uk/rpi/pigpio/) and [libgpiod](https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/). Might be good to do this in the Apama GPIO library so it's more widely useful.
- Investigate using the kernel SPI API with the `spi-gpio` driver to eliminate the current DIY bit-banging approach. There is an open [issue](https://github.com/pimoroni/blinkt/issues/65) in the Pimoroni Python driver project too. Again this might be something to push into the Apama GPIO library.
