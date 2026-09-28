# Overview
During this lab we will be learning the design flow of using the Zynq SoC. By completing this lab we should be able to create a Zynq hardware project, configure a Zynq PS, and have interaction with the Zynq PS and PL side.

# Design Summary
Starting from the PL side we will create a Zynq SoC design using Vivado Block Design Feature. Next, we will need to make sure we have multiple AXI GPIO peripherals so we can interact with RGB LEDS, LEDS and SWITCHES.

# Verification and Testing
There is no TestBench for this Lab.
# Known Issues and Limitations
When it comes to instantiating more AXI_GPIO Block Designs. Then running the block automation, then run connection automation, then validate design, the HDL Wrapper must be done first then generate the bistream and then export the hardware bitstream into a .xsa file. Then upload it into the Vitis 2023.1. Skipping the HDL wrapper will cause issues when it comes to the Vitis 2023.1 software because there will be some missing files in order for the hardware interaction can be succesful.
# References
ece520L_fall26_lab2_axi_gpio.pdf
