# ESP8266 Swarm Feeds client sample

This is a quick-and-dirty sample on how to connect an ESP8266 to Swarm to publish sensor data. It reads A0 ADC pin to calculate a temperature out of a voltage reading. 

When run on a device, it will read A0 and publish it to a feed periodically.

Don't expect anything production-ready. The main feat here was to get signatures right, which implied modifying the ethers/uECC library so it would compute them correctly. I tried packaging all feeds-related code in a library "swarmfeed" which I will eventually segregate into its own repository if this sample picks up interest.

# How to run this

You will need Platform.IO to compile this. Other ways are possible, I am sure.

Attach a thermistor to ESP8266's A0 with a voltage divider. Note: this scheme works for most development modules. Other modules may need a max 1V input to A0. Check your module specs.

Vcc(3.3v) --- thermistor --- A0 ---- 4.7k ohm resistor ---- GND

You can also attach some other thing to A0 or modify the sample to read other data.

Modify `lib/config/config.h` to match your Wifi configuration and set up a private key. This is an Ethereum private key. For Feeds, this private key **does not** need to have Ether, so you can create as many as you need.

Compile and upload to your ESP8266.

## www folder

There is also a very simple HTML application that plots the data published by the device on Swarm. You will notice a few config parameters in `www/index.html`. Make sure you put the Ethereum Address corresponding to the private key you put in `config.h`.


# Final notes

I will improve this sample with your feedback, find me in Gitter: @jpeletier. Feel free to open PRs to improve this code.

I am not responsible if you break your hardware! Check your connections and make sure you know what you're doing