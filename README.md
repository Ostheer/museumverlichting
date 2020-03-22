# museumverlichting
C++ Telegram bot for controlling the addressable led strip in my museum

Start bot on Raspberry Pi with `sudo ./main YOURTOKEN`

# Problems
Because the library uses the PWM GPIO pin (GPIO18), which is addressed via `/dev/mem`, the program needs to run as root.

I've tried to get around this by using SPI instead, but both https://github.com/jgarff/rpi_ws281x/ and https://github.com/SIGSEGV111/ws281x-spi-driver do not give the desired results. The former manages to set the leds sporadically (about once every ten `render` invocations), crashing itself and the ssh session every now and then, whereas the latter seems to function fine.... as long as you're running it as root.
