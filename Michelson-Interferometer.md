### 1. Project Overview

This project builds a functional **Michelson interferometer** to measure extremely small changes in distance, relying on the interference pattern of a green laser. The **ESP32** microcontroller will be used to read and interpret the light intensity from a photodetector, allowing you to quantify changes in the optical path in units of a fraction of a wavelength.

***

### 2. Parts List

#### Optical Components
* **Green Diode Laser Module (532nm):** A stable, low-power green laser.
* **Beam Splitter Cube:** A 50/50 beam splitter cube is ideal, but a partially silvered mirror or even a glass slide will work for a basic demonstration.
* **Mirrors (x2):** Small, flat, first-surface mirrors are best to avoid ghost reflections.
* **Mounts and Stands:** Adjustable mounts for the laser, mirrors, and beam splitter. You'll also need a stable, rigid base (an optical breadboard, a piece of wood, or thick aluminum plate).

#### Electronic Components
* **ESP32 Development Board:** A common model like the ESP32 DevKitC is perfect.
* **Photodiode:** A photodiode (e.g., BPW34) is a fast and sensitive light sensor. A photoresistor (LDR) is a cheaper alternative but is much slower.
* **Resistor (10 kΩ):** For the photodiode circuit.
* **Breadboard and Jumper Wires:** For connecting the components.
* **Stepper Motor (optional):** A small stepper motor with a driver (e.g., A4988) can be used to precisely move one of the mirrors.


***

### 3. Assembly and Alignment

1.  **Mount Components:** Securely attach all optical components to your stable base. The laser, beam splitter, and two mirrors should be arranged in the classic "L" shape of a Michelson interferometer.
2.  **Initial Alignment:**
    * Place the laser and beam splitter so the beam is split into two perpendicular paths.
    * Position one mirror at the end of each path. Carefully adjust the mirrors so the two reflected beams recombine on top of each other.
    * The overlapping beams will create a visible interference pattern of concentric light and dark rings on a screen.
3.  **Photodetector Placement:** Place the photodetector at the point where the two beams recombine. This is the output of the interferometer.
4.  **Wiring:**
    * **Photodiode:** Connect the photodiode in series with a 10 kΩ resistor. Connect the junction of the photodiode and resistor to an analog input pin on the ESP32 (e.g., `GPIO34`). Connect the other ends to `3.3V` and `GND` as appropriate for your photodiode type.

***

### 4. Sample ESP32 Arduino Sketch

The following code reads the analog voltage from the photodiode and prints it to the serial monitor. This value will change as the interference pattern shifts.

```cpp
const int photoPin = 34; // GPIO34 is a good choice for analog input on ESP32

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Read the analog value from the photodiode
  int sensorValue = analogRead(photoPin);
  
  // Print the value to the Serial Monitor
  Serial.println(sensorValue);
  
  // A small delay to keep the serial output readable
  delay(10);
}
