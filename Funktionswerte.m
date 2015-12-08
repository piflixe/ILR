% Skript zur Erstellung eines Spaltenvektors mit Funktionswerten

% Frequenz
f = 150; % gewünschte Frequenz [Hz]
f_s = 13e3; % gewünschte Abtastrate [Hz]
A = 0.2; % Amplitude des Ausgangssignals [V]
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
y = ((2^bit_DAC)-1) / V_ref * (A*sin(2*pi*f*t)+A) + offset;

% auf ganze Werte runden
y=round(y);

%% Abspeichern
disp(['speichere Daten in Datei ',filename,' ...']);
disp(['Frequenz: ',int2str(f),'Hz']);
disp(['Abtastfrequenz: ',int2str(f_s),'Hz']);
disp(['... das entspricht ',int2str(f_s/f), ' Sample pro Periode']);

fID = fopen(filename,'w+');
% header schreiben
fprintf(fID, '#ifndef _Funktionswerte_h_\n');
fprintf(fID, '#define _Funktionswerte_h_\n\n');

% parameter schreiben 
% Nval muss auf jeden Fall const sein, weil sonst arrays damit nicht 
% initalisiert werden können
fprintf(fID, 'const unsigned int Nval = %g; // number of entries in outputSignal\n', length(y));
fprintf(fID, 'unsigned int Tsmic = %g; // sampling rate [micro s]\n', T_s * 1e6);
fprintf(fID, 'const unsigned int Tmic = %g; // length of one period in outputSignal [micro s]\n\n', T * 1e6);

fprintf(fID, 'const unsigned int table[] = {\n');
for i=1:(length(y)-1)
    fprintf(fID, '%4.0f,',y(i)); % alle bis auf den letzten Wert schreiben
    if(mod(i,15)==0)
        fprintf(fID, '%1s\n', ' '); % Umbruch alle soundsoviel Zeilen
    end
end
fprintf(fID, '%4.0f\n',y(end)); % nur den letzten Wert schreiben (kein KOMMA)
fprintf(fID, '};\n\n');
fprintf(fID, '#endif\n'); % footer schreiben

fclose(fID);