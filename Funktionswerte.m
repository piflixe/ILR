%% script to automatically create values for ILC
% in sine waveform
%
% Felix Piela
% Feb 2016

% Frequenz
f = 368;        % desired frequency [Hz]
f_s = 30e3;     % desire sample rate [Hz]
A = 0.4;        % desired amplitude [V]
DCfactor = 0.5; % factor for DC offset (0.5 for midrange)

% DAC properties
V_ref = 3.3;    % reference voltage [V]
bit_DAC = 12;   % DAC resolution
offset = 2^bit_DAC * DCfactor; % calculate integer offset value for DAC

% filename for output
filename = './ILR/Funktionswerte.h';

% calculate periods
T = 1/f;
T_s = 1/f_s;

t = linspace(0, T-T_s,  T/T_s);

% calculate values
y = ((2^bit_DAC)-1) / V_ref * (A*sin(2*pi*f*t)) + offset;
y=round(y); % round to integer values

%% save values 
saveValuesToFile(filename, f, f_s, T, T_s, y, t);