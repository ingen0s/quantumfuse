# Adiabatic Demagnetization Refrigeration (ADR) Cryostat Build Guide

## Objective

Construct a compact, single-stage ADR cryostat to cool a 50 mm x 50 mm x 1 mm quartz crystal to below 4 K (target ~0.1–1 K) for low-temperature experiments. This design leverages paramagnetic salt demagnetization, pre-cooled by liquid helium (LHe), using commercially available components from Amazon, eBay, and AliExpress. Estimated cost: $1,500–$5,000; assembly time: 1–2 weeks.

## Safety Considerations

- **Cryogenic Hazards**: Liquid helium risks asphyxiation (use oxygen monitors) and cold burns (wear PPE: cryogenic gloves, face shield).
- **Magnetic Fields**: High fields (2–4 T) can attract tools or affect medical devices; ensure a field-safe zone.
- **Vacuum Safety**: Check chamber integrity to avoid implosion.
- Always test with non-critical components first.

## Components

|Component|Description|Source|Approx. Cost|
|---|---|---|---|
|**Paramagnetic Salt Pill**|100–200 g chrome alum (chromium potassium sulfate) in sealed copper tube for spin-based cooling.|eBay (chemical suppliers)|$25–$40|
|**Magnet System**|Neodymium N52 blocks (40x20x10 mm, stack 4–6 for ~2 T) or solenoid coil for 2–4 T field.|AliExpress (magnets); eBay (coils)|$50–$100|
|**Vacuum Chamber**|3-gallon stainless steel chamber with acrylic lid and 3.5 CFM pump for thermal isolation.|Amazon|$150–$250|
|**Liquid Helium Dewar**|Used 30 L Cryofab CMSH-30 for pre-cooling to ~4 K.|eBay (local pickup preferred)|$500–$1,000|
|**Temperature Sensor**|Lake Shore Cernox (100 mK–420 K range, low magnetic error).|eBay or Lake Shore|$200–$500|
|**PID Controller & Heater**|Inkbird PID kit with SSR, adapted with 50 Ω resistor for temperature stabilization.|Amazon|$40–$60|
|**Heat Switch**|Superconducting NbTi wire or mechanical link to toggle thermal contact.|eBay (superconductor samples)|$20–$50|
|**Miscellaneous**|OFHC copper block, thermal grease (Apiezon N), copper braid, G-10 supports, MLI (aluminized Mylar).|Amazon/AliExpress|$50–$100|

**Note**: Liquid helium sourced locally (e.g., Airgas, ~$20–$50/L). Total helium use: ~0.5–1 L per run.

## Assembly Instructions

### 1. Fabricate the Salt Pill

- **Materials**: 100–200 g chrome alum, copper tube (1–2 cm dia, 5–10 cm long, AliExpress ~$10), epoxy.
- **Process**:
    1. Dissolve chrome alum in distilled water, heat to concentrate, and pour into copper tube.
    2. Allow crystallization (slow cooling for large crystals).
    3. Seal tube ends with non-magnetic epoxy; ensure vacuum-tight.
- **Purpose**: Forms the paramagnetic core for heat absorption via spin disordering.

### 2. Construct the Cold Stage

- **Materials**: OFHC copper block (20x20x10 mm), thermal grease, G-10 fiberglass rods.
- **Process**:
    1. Machine copper block to hold quartz crystal (50x50x1 mm); secure with Apiezon N grease for thermal contact.
    2. Attach salt pill to block via copper braid (solder for conductivity).
    3. Suspend assembly in vacuum chamber on G-10 rods to minimize heat leaks.
    4. Mount Cernox sensor and 50 Ω heater resistor on block; wire to feedthroughs.

### 3. Set Up Vacuum and Magnet Systems

- **Vacuum Chamber**:
    1. Assemble 3-gallon chamber with pump; evacuate to 10^-4 mbar.
    2. Line inner walls with 10–20 layers of aluminized Mylar (MLI) for radiation shielding.
- **Magnet**:
    1. Position neodymium stack (or solenoid) around chamber exterior for uniform 2–4 T field.
    2. Secure with non-magnetic clamps; connect solenoid to DC power supply (30V/10A, Amazon ~$50) if used.

### 4. Integrate Pre-Cooling and Electronics

- **Pre-Cooler**:
    1. Connect vacuum chamber to LHe dewar via flexible stainless steel transfer line (AliExpress ~$30).
    2. Test for leaks with helium gas before LHe transfer.
- **Electronics**:
    1. Wire PID controller to sensor/heater via chamber feedthroughs.
    2. Calibrate at room temperature and 77 K (liquid nitrogen test).

## Operational Procedure

The ADR cycle is single-shot, cooling from ~4 K to 0.1–1 K for 10–60 minutes per run. Repeat as needed.

1. **Pre-Cool**:
    
    - Transfer LHe to chamber bath/coils until system reaches ~~4 K (~~1–2 hrs, ~0.5 L).
    - Use PID controller to stabilize temperature.
2. **Magnetize (Isothermal)**:
    
    - With heat switch ON (NbTi wire superconducting or mechanical contact closed), ramp magnetic field to 2–4 T (0.1 T/min to avoid eddy currents).
    - Salt pill heats (~1–2 K); heat dissipates to LHe bath.
3. **Isolate**:
    
    - Open heat switch (raise NbTi above Tc with small current or disengage mechanical link), thermally isolating cold stage.
4. **Demagnetize (Adiabatic)**:
    
    - Slowly reduce field to 0 T (over 10–30 min). Spin disorder absorbs heat (~100–500 mJ), cooling stage to 0.1–1 K.
    - Quartz crystal thermalizes rapidly due to low mass.
5. **Experiment**:
    
    - Conduct measurements (e.g., quartz resonance properties) at base temperature.
    - Monitor with Cernox; stabilize with heater if needed.
6. **Recycle**:
    
    - Re-magnetize with switch ON to warm pill; reconnect to LHe bath.
    - Repeat cycle or warm system by stopping LHe flow.

## Schematics

### System Cross-Section

```
+-----------------------------+   <-- Vacuum Chamber (Stainless Steel)
|                             |
|   +-------------------+     |   <-- Magnetic Shield (Optional)
|   | Magnet (Neodymium)|     |   <-- 2–4 T Field
|   | or Solenoid       |     |
|   +-------------------+     |
|            |                |
|   +-------------------+     |   <-- Salt Pill (Chrome Alum)
|   | Paramagnetic Pill |     |
|   +-------------------+     |
|            |                |
|   +-------------------+     |   <-- Heat Switch (NbTi Wire)
|   | Heat Switch       |     |
|   +-------------------+     |
|            |                |
|   +-------------------+     |   <-- Cold Stage (Cu Block)
|   | Quartz Crystal    |     |
|   | Sample Holder     |     |
|   +-------------------+     |
|            |                |
|   +-------------------+     |   <-- LHe Bath (~4 K)
|   | Pre-Cooler (LHe)  |     |
|   +-------------------+     |
|                             |
+-----------------------------+
   ^ Feedthroughs (Sensor, Heater, PID)
```

### ADR Cycle Flowchart

```
Start
  |
  v
Pre-Cool to ~4 K (LHe Dewar)
  |
  v
Magnetize (2–4 T, Switch ON) --- Heat to LHe
  |
  v
Isolate (Switch OFF)
  |
  v
Demagnetize (Field to 0 T) ---> Cool to 0.1–1 K
  |
  v
Hold & Measure (Quartz at Low T)
  |
  v
Re-Magnetize & Reconnect ---> Repeat
```

## Performance Metrics

- **Base Temperature**: 0.1–1 K (dependent on field strength and heat leaks).
- **Cooling Time**: ~1–2 hrs to 4 K; ~10–30 min for demagnetization to base.
- **Hold Time**: 10–60 min before heat leaks warm system.
- **Helium Use**: ~0.5–1 L per run.
- **Stability**: ±5 mK with PID tuning.

## Challenges & Mitigations

- **Heat Leaks**: Target <1 μW via high vacuum and MLI. Test without field first.
- **Magnetic Field**: Neodymium stack limits to ~2 T; solenoid or used superconducting magnet (eBay ~$1,000) for <0.5 K.
- **Helium Supply**: Local sourcing critical; consider recovery pump for cost (~$200, Amazon).
- **Crystal Mounting**: Ensure thermal contact with grease; test with indium foil if needed.

## Resources

- **Suppliers**: eBay (dewars, sensors), Amazon (PID, vacuum), AliExpress (magnets, tubing).
- **References**: “Experimental Techniques in Low-Temperature Physics” (White & Meeson); online cryogenic forums.
- **Next Steps**: Validate with dummy sample (copper plate); consult local suppliers & physics labs for LHe handling.
