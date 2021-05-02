# CSLight

CSLight is my kitchen coffee-station worklight. That part of the kitchen is a little darker than I'd like, so I had the idea - let's use a [NeoPixel](https://www.adafruit.com/product/1586) ring as a worklight, and get it on the network, so it can be controlled from my [Lights](https://github.com/kenkl/lights) app. Theoretically, it should be able to control it from any "smart speaker" thing, as long as it can make local REST calls. CSLight borrows heavily from [ESP_TempThing](https://github.com/kenkl/ESP_TempThing), adapted to [ESP32](https://www.adafruit.com/product/4172) (previously ESP8266), and implementing behaviours for light brightness, colour, and colour-temperature.


### Changelog

2021-02-02: Initial version.

2121-05-02: 

- Fiddling around with the colour-temperature.
- First commit to GitHub. 
