# Spool Holder with Integrated RFID Reader

This is a spool holder with an integrated RFID reader. It reads a custom RFID tag on the spool and tracks filament quantity in connection with a cloud service. The quantity is tracked based on tasks running through the cloud service. This system helps me efficiently manage the remaining filament on spools, optimizing task distribution among printers.

## Compatibility
This spool holder is designed to be used with **JetForge**, a 3D printing business and farm management software. However, you are free to modify the firmware as needed.

**JetForge Status:** Currently in beta testing.
- A free plan for hobbyists and small businesses will be available.
- Contact me for beta access: [sergey@jetforge.app](mailto:sergey@jetforge.app)

## Few words about JetForge

It is 3D print farm and business management software, designed for all sizes of 3D printing business (but on first hand keeping in mind garage businesses). It allows to import orders from Shopify and Etsy stores and to match automatically ordered items to files, keeping in mind printer models and materials. It calculates cost for each order taking into account material price, 3d printer hourly cost and energy consumption (using inexpensive zigbee smart power plugs). There's a huge roadmap including AI features, more storefront engines to support, material stock tracking and printer maintenance schedules. I'm will be happy to accept beta testers to checkout the software and give feedback. It's your exclusive opportunity to participate in product creation.

## Hardware Components

### Main Board
- **ESP32-DEVKIT** (Micro USB 38-pin version)
  - [AliExpress Link 1](https://www.aliexpress.com/item/1005007867820740.html)
  - [AliExpress Link 2](https://www.aliexpress.com/item/1005008370870784.html)
  - *(You may use the Type-C version or modify the case holes for different adapters.)*

### Additional Components
- **Breakout board for ESP32-DEVKIT**  
  [AliExpress Link](https://www.aliexpress.com/item/1005006026098254.html)
- **1.3" OLED screen**  
  [AliExpress Link](https://www.aliexpress.com/item/1005007451015054.html)
- **MFRC-522 RFID module**  
  [AliExpress Link](https://www.aliexpress.com/item/1005007655966167.html)
- **Extension cable with Type-C socket**  
  [AliExpress Link](https://www.aliexpress.com/item/1005006988168288.html)
- **Button**  
  [AliExpress Link](https://www.aliexpress.com/item/1005003423084423.html)
- **BTT SFS V2.0 Smart Filament Sensor**  
  [BIQU Store](https://biqu.equipment/products/btt-sfs-v2-0-smart-filament-sensor)
- **3mm screws for thermoplastics**
  [TME Link](https://www.tme.eu/lv/ru/details/b3x20_bn2041/shurupy/bossard/3757664/)
- **Ribbon cable** *(Color scheme matches the pinout.pdf)*  
  [AliExpress Link](https://www.aliexpress.com/item/4001132905947.html)

## Wiring
- The provided ribbon cable follows the color scheme in pinout.pdf file
- You can use **2.54mm pin headers** or **solder wires directly** to the RFID and screen module PCBs.

## Contribution & Feedback
Feel free to contribute or suggest improvements! Open an issue on the repository if you encounter any issues.
