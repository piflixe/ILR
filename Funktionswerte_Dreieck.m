% Skript zur Erstellung eines Spaltenvektors mit Funktionswerten

% clear up existing variables
clear y t

% Frequenz
f = 2; % gewünschte Frequenz [Hz]
f_s = 80; % gewünschte Abtastrate [Hz]
A = 0.3; % Amplitude des Ausgangssignals [V]
PWM = 0.8; % Pulsweite 

% Daten des DAC
V_ref = 3.3; % verwendete Referenzspannung DAC [V]
bit_DAC = 12; % wieviel bit Auflösung hat der DAC?

% Dateiname zum speichern
filename = './ILR/Funktionswerte.h';

offset=2^bit_DAC / 2;

% Perioden ausrechnen
T = 1/f;
T_s = 1/f_s;

t = linspace(0, T-T_s,  T/T_s);

% Stützpunkte für Dreiecksignal:
yBase(1) = -1;
yBase(2) = 1;
yBase(3) = -1;
xBase(1) = 1;
xBase(2) = round( PWM * length(t) );
xBase(3) = length(t);

% Dreiecksignal berechnen (normiert)
y(xBase(1):xBase(2)) = interp1(t(xBase(1:2)),yBase(1:2),t(xBase(1):xBase(2)),'linear');
y(xBase(2):xBase(3)) = interp1(t(xBase(2:3)),yBase(2:3),t(xBase(2):xBase(3)),'linear');

% um halbe Amplitde verschieben
y = ((2^bit_DAC)-1) * A / V_ref  * y + offset;

% auf ganze Werte runden
y=round(y);

%% Abspeichern
saveValuesToFile(filename, f, f_s, T, T_s, y, t);