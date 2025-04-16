# VeinFinder Project

## Overview
The VeinFinder project is an Arduino-based system designed to control LEDs, sensors, and buttons to assist in vein detection. The project is structured to modularize functionality into tasks for better maintainability and scalability.

## Project Structure
```
platformio.ini
include/
	button_led.h
	led_control.h
	pins.h
	README
	sensor.h
lib/
	README
src/
	button_led.cpp
	led_control.cpp
	main.cpp
	sensor.cpp
test/
	README
```

### Key Directories and Files
- **include/**: Contains header files for various modules.
- **lib/**: Placeholder for external libraries.
- **src/**: Contains the main source code files.
- **test/**: Placeholder for test-related files.

## Setup and Usage
1. **Install PlatformIO**: Ensure you have PlatformIO installed in your development environment.
2. **Open the Project**: Open the project folder in your IDE (e.g., VS Code).
3. **Build and Upload**: Use PlatformIO to build and upload the code to your Arduino-compatible board.

## Main Features
- **LED Control**: Manage LED patterns and brightness.
- **Sensor Integration**: Detect sensor input for vein detection.
- **Button Control**: Handle button inputs for user interaction.

## Code Overview
### `main.cpp`
The `main.cpp` file initializes the system and runs the main loop. Key functions include:
- `setup()`: Initializes pins, LEDs, sensors, and buttons.
- `loop()`: Monitors system status and prints debug information to the Serial Monitor.

### Tasks
- **LED Task**: Manages LED patterns and brightness.
- **Sensor Task**: Handles sensor input and detection.
- **Button Task**: Processes button inputs.

## Dependencies
- **Arduino Framework**: Used for core functionalities.
- **SPI Library**: For communication with peripherals.

## Contributing
Contributions are welcome! Please follow the standard Git workflow:
1. Fork the repository.
2. Create a feature branch.
3. Commit your changes.
4. Submit a pull request.

## License
This project is licensed under the MIT License. See the LICENSE file for details.