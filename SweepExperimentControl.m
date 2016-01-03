% script to control sweep experiments
%
% author: Felix Piela
% dec 2015
%

% Arduino needs to be configured so that Serial Console doesn't return
% anything. (Proper Messages over Serial still in progress 
% see https://github.com/piflixe/ILR/issues/5 

%% HEADER
sampleFreq_start = 20000;
sampleFreq_end = 21000;

%% SET UP
ILR_Serial = serial('COM8'); % Create Serial Port
set (ILR_Serial, ...  % Set Serial Port Properties
    'BaudRate',115200, ... % set BAUD rate
    'terminator','LF' ... % set end of message to Line Feed (Arduino)
);

%% INIT ARDUINO
fopen(ILR_Serial); % Open Serial Port
pause(0.3); % seems to need some time to settle

fprintf(ILR_Serial,'2'); % go to sampleFreq menu
fprintf(ILR_Serial,num2str(sampleFreq_start)); % set to start frequency

fprintf(ILR_Serial,'3'); % go to Ki menu % function not implemented
fprintf(ILR_Serial,'70'); % set Ki value

fprintf(ILR_Serial,'4'); % go to Kp menu % function not implemented
fprintf(ILR_Serial,'150'); % set Kp value

disp('starting experiment...');
fprintf(ILR_Serial,'1'); % go to start menu 
% pause(0.01);
fprintf(ILR_Serial,'1'); % start experiment
disp('waiting for start frequency to settle...');
pause(2);

%% SWEEP
disp('start sweep')
for i = 2:2:(sampleFreq_end-sampleFreq_start)
    fprintf(ILR_Serial,'2'); % go to sample rate menu
    %pause(0.1);
    fprintf(ILR_Serial,num2str(sampleFreq_start + i)); % set new sample rate
    disp('current sample frequency: ');
    disp(sampleFreq_start + i);
    pause(1);
end

disp('end sweep');
pause(2);

%% 
disp('ending experiment and closing connection');
fprintf(ILR_Serial,'1'); % go to start menu 
% pause(0.01);
fprintf(ILR_Serial,'3'); % stop experiment
pause(1);

fclose(ILR_Serial); % close Serial Port
delete(ILR_Serial); % deleting Serial Port Object
clear ILR_Serial;
