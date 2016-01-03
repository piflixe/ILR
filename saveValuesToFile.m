function error = saveValuesToFile(filename, f, f_s, T, T_s, y, t)
% Function for saving data to a arduino readable header File
% PROGMEN functionality to be added later
% 
% author: Felix Piela
% 2015
%
% part of master thesis in iterative learning control
%
% USAGE
% saveValuesToFile(filename, f, f_s, T, T_s, y, t)
% 
% returns 0 if everything worked fine (to be improved)

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

error = 0;
end