% Skript zur Erstellung eines Spaltenvektors mit Funktionswerten

% Frequenz
f = 150; % gewünschte Frequenz [Hz]
f_s = 20e3; % gewünschte Abtastrate [Hz]
A = 0.3; % Amplitude des Ausgangssignals [V]
DCfactor = 0.5; % Factor for DC offset (0.5 for midrange)

% Daten des DAC
V_ref = 3.3; % verwendete Referenzspannung DAC [V]
bit_DAC = 12; % wieviel bit Auflösung hat der DAC?
offset = 2^bit_DAC * DCfactor; % calculate integer offset value for DAC

% Dateiname zum speichern
filename = './ILR/Funktionswerte.h';

% Perioden ausrechnen
T = 1/f;
T_s = 1/f_s;

t = linspace(0, T-T_s,  T/T_s);

% um halbe Amplitde verschieben
y = ((2^bit_DAC)-1) / V_ref * (A*sin(2*pi*f*t)) + offset;

% auf ganze Werte runden
y=round(y);

%% Abspeichern
saveValuesToFile(filename, f, f_s, T, T_s, y, t);