# 3D Printed ESP32-S3 Steno Keyboard

![Finished Keyboard](https://github.com/s-downs/esp-steno-kb/blob/main/Images/Finished%20Keyboard.jpg?raw=true)

I wanted to look at learning steno and didn't have an NKRO keyboard, but what I did have was an ESP32 S3 devkit module and a 3D printer, so I ordered some Kailh Choc v1 switches and keycaps from Aliexpress and built one - Overall this project cost probably about £35 (~40 USD) and took a little under 2 days. I now have the "right" keyboard (if there is such a thing) to start with and actually used that 3D printer and soldering iron for a change!

## Required materials

* ESP32 S3 Devkit module
* 3D Printer (or access to one)
* Soldering iron (or access to one)
* Bare copper wire to form the ground connection
* Various lengths of sheathed wire to form the button connections.
* 20x M4 screws (up to ~10mm length)
* 20x M4 inserts (6mm outer diameter, up to ~8mm length)
* 28x Choc 1350 v1 switches (square 13.4mm base)
* 28x Choc 1350 keycaps (17.4mm width, 16.4mm high - Spacing is 18mm horizontal, 17mm vertical)
* Hot glue gun or double sided tape

## Printing the parts

If you don't need to make any changes to the parts being used to create your keyboard, the STL files should be ready to print! Depending on which surface you want to be visible the parts might need flipping in your slicer of choice (the sides *are not* symmetrical). On my Ender 3 it took around 7 and a half hours to print all 5 parts (~40mins each for the plates, ~2hr 20mins for the right body pieces, and ~1hr 20mins for the central support).

If any edits need to be made feel free to edit the .blend files also included, the naming of all the parts should be fairly self explanatory.

## How to build

1. Print the left and right switch panels and bodies, as well as the central support (which includes the space for the ESP32).
2. (optional) Remove the Rx and Tx LEDS from your ESP32 module.
3. Ensure the switch panels fit on the bodies nicely, and that there aren't any obvious printing errors (the switch panel doesn't fit into, but instead lays ontop of, the body).
4. Use a soldering iron or threaded insert tool to fit inserts into the 7 holes in the body of each side, and the 6 holes in the central support.
5. Test fit the 3D printed parts to make sure they all work together.
6. Press in the Choc 1350 switches, the print is designed so that they should clip in on their own, however a little superglue may be used to further secure them.
7. Remove bodies, but leave the 2 plates connected to, and via, the central support.
8. Connect all of the switches on each side using a bare piece of copper wire, this will form the ground for the switches.
9. Place your ESP32 into the central support.
10. Solder a sheathed wire from the bare copper ground on each side to the ESP32.
11. Solder the remaining pin of each switch to the ESP32, ensuring you do not connect any switches to pins 0, 19 or 20 (these are used for the boot button, and USB), or the pin used to drive the data pin of the WS2812 LED on the module.
12. Update ```const uint8_t inputKeys``` in the Arduino sketch to match how the switches have been wired and what you want each to do.
13. Plug the ESP32 into your computer, using the "OTG" port of the board. 
14. Press and hold the "BOOT" button on your module, and while having that pressed down press the "RST" button before releasing it. The "BOOT" button can now be released.
15. The module should now allow you to flash it via the "OTG" port using it's built in serial adapter.
16. Enjoy!

## Modifying the keyboard library to enable full rollover

By default the keyboard library included in Arduino only offers 6 key rollover, so we need to modify it to enable full rollover capability, find the ESP32 packages folder that (on my machine is at) ```%LocalAppData%\Arduino15\packages\esp32\``` and edit the following files (with relative paths).


```cpp
// %LocalAppData%\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\USB\src\USBHIDKeyboard.h

typedef struct
{
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[30]; // <-- Change this from 6 to 30
} KeyReport;
```

```cpp
// %LocalAppData%\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\USB\src\USBHIDKeyboard.cpp

void USBHIDKeyboard::sendReport(KeyReport* keys)
{
    hid_keyboard_report_t report;
    report.reserved = 0;
    report.modifier = keys->modifiers;
    if (keys->keys) {
        memcpy(report.keycode, keys->keys, 30);	// <-- Change this from 6 to 30.
    } else {
        memset(report.keycode, 0, 30);	// <-- Change this from 6 to 30.
    }
    hid.SendReport(HID_REPORT_ID_KEYBOARD, &report, sizeof(report));
}

// ...

size_t USBHIDKeyboard::pressRaw(uint8_t k) 
{
	// ...
	
	// vvv Change this if statement
	if (_keyReport.keys[0] != k && _keyReport.keys[1] != k && 
		_keyReport.keys[2] != k && _keyReport.keys[3] != k &&
		_keyReport.keys[4] != k && _keyReport.keys[5] != k &&
		_keyReport.keys[6] != k && _keyReport.keys[7] != k &&
		_keyReport.keys[8] != k && _keyReport.keys[9] != k &&
		_keyReport.keys[10] != k && _keyReport.keys[11] != k &&
		_keyReport.keys[12] != k && _keyReport.keys[13] != k &&
		_keyReport.keys[14] != k && _keyReport.keys[15] != k &&
		_keyReport.keys[16] != k && _keyReport.keys[17] != k &&
		_keyReport.keys[18] != k && _keyReport.keys[19] != k &&
		_keyReport.keys[20] != k && _keyReport.keys[21] != k &&
		_keyReport.keys[22] != k && _keyReport.keys[23] != k &&
		_keyReport.keys[24] != k && _keyReport.keys[25] != k &&
		_keyReport.keys[26] != k && _keyReport.keys[27] != k &&
		_keyReport.keys[28] != k && _keyReport.keys[29] != k ) {
		
		for (i=0; i<30; i++) { // <-- Change this from 6 to 30.
			if (_keyReport.keys[i] == 0x00) {
				_keyReport.keys[i] = k;
				break;
			}
		}
		if (i == 30) { // <-- Change this from 6 to 30.
			return 0;
		}   
	}
	
	// ...
}

// ...

size_t USBHIDKeyboard::releaseRaw(uint8_t k) 
{
	// ...
	
	for (i=0; i<30; i++) { // <-- Change this from 6 to 30.
		if (0 != k && _keyReport.keys[i] == k) {
			_keyReport.keys[i] = 0x00;
		}
	}
	
	// ...
}

// ...

void USBHIDKeyboard::releaseAll(void)
{
	for (int i=0; i<30; i++) { // <-- Change this from 6 to 30.
		_keyReport.keys[i] = 0;
	}
    sendReport(&_keyReport);
}
```

```cpp
// %LocalAppData%\Arduino15\packages\esp32\hardware\esp32\2.0.11\tools\sdk\esp32s3\include\arduino_tinyusb\tinyusb\src\class\hid\hid.h

typedef struct TU_ATTR_PACKED
{
  uint8_t modifier;
  uint8_t reserved;
  uint8_t keycode[30]; // <-- Change this from 6 to 30.
} hid_keyboard_report_t;
```

```cpp
// %LocalAppData%\Arduino15\packages\esp32\hardware\esp32\2.0.11\tools\sdk\esp32s3\include\arduino_tinyusb\tinyusb\src\class\hid\hid_device.h

bool tud_hid_n_keyboard_report(uint8_t instance, uint8_t report_id, uint8_t modifier, uint8_t keycode[30]); // <-- Change this from 6 to 30.

// ...

static inline bool    tud_hid_keyboard_report(uint8_t report_id, uint8_t modifier, uint8_t keycode[30]); // <-- Change this from 6 to 30.

// ...

static inline bool tud_hid_keyboard_report(uint8_t report_id, uint8_t modifier, uint8_t keycode[30]) // <-- Change this from 6 to 30.
{
  return tud_hid_n_keyboard_report(0, report_id, modifier, keycode);
}

// ...

#define TUD_HID_REPORT_DESC_KEYBOARD(...) \
HID_USAGE_PAGE ( HID_USAGE_PAGE_DESKTOP     )                    ,\
HID_USAGE      ( HID_USAGE_DESKTOP_KEYBOARD )                    ,\
HID_COLLECTION ( HID_COLLECTION_APPLICATION )                    ,\
__VA_ARGS__ \
HID_USAGE_PAGE ( HID_USAGE_PAGE_KEYBOARD )                     ,\
  HID_USAGE_MIN    ( 224                                    )  ,\
  HID_USAGE_MAX    ( 231                                    )  ,\
  HID_LOGICAL_MIN  ( 0                                      )  ,\
  HID_LOGICAL_MAX  ( 1                                      )  ,\
  HID_REPORT_COUNT ( 8                                      )  ,\
  HID_REPORT_SIZE  ( 1                                      )  ,\
  HID_INPUT        ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE )  ,\
  HID_REPORT_COUNT ( 1                                      )  ,\
  HID_REPORT_SIZE  ( 8                                      )  ,\
  HID_INPUT        ( HID_CONSTANT                           )  ,\
HID_USAGE_PAGE  ( HID_USAGE_PAGE_LED                   )       ,\
  HID_USAGE_MIN    ( 1                                       ) ,\
  HID_USAGE_MAX    ( 5                                       ) ,\
  HID_REPORT_COUNT ( 5                                       ) ,\
  HID_REPORT_SIZE  ( 1                                       ) ,\
  HID_OUTPUT       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE  ) ,\
  HID_REPORT_COUNT ( 1                                       ) ,\
  HID_REPORT_SIZE  ( 3                                       ) ,\
  HID_OUTPUT       ( HID_CONSTANT                            ) ,\
HID_USAGE_PAGE ( HID_USAGE_PAGE_KEYBOARD )                     ,\
  HID_USAGE_MIN    ( 0                                   )     ,\
  HID_USAGE_MAX_N  ( 255, 2                              )     ,\
  HID_LOGICAL_MIN  ( 0                                   )     ,\
  HID_LOGICAL_MAX_N( 255, 2                              )     ,\
  HID_REPORT_COUNT ( 30                                  )     ,\ // <-- Change this from 6 to 30.
  HID_REPORT_SIZE  ( 8                                   )     ,\
  HID_INPUT        ( HID_DATA | HID_ARRAY | HID_ABSOLUTE )     ,\
HID_COLLECTION_END \
```
