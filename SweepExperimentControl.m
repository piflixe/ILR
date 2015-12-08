% script to control sweep experiments
%
% author: Felix Piela
% dec 2015
%

% Arduino needs to be configured so that Serial Console doesn't return
% anything. (Proper Messages over Serial still in progress 
% see https://github.com/piflixe/ILR/issues/5 

start_T = 73;
end_T = 95;

ILR_Serial = serial('COM8'); % Create Serial Port
set (ILR_Serial, ...  % Set Serial Port Properties
    'BaudRate',115200, ... % set BAUD rate
    'terminator','LF' ... % set end of message to Line Feed (Arduino)
);

fopen(ILR_Serial); % Open Serial Port
pause(0.3); % seems to need some time to settle

fprintf(ILR_Serial,'2'); % go to Tsmic menu
fprintf(ILR_Serial,num2str(end_T)); % set Tsmic to start frequency

fprintf(ILR_Serial,'4'); % go to ILCgain menu % function not implemented
fprintf(ILR_Serial,'50'); % set ILCgain value

% fprintf(ILR_Serial,'3'); % go to ILCgain menu % function not implemented
% fprintf(ILR_Serial,'180'); % set ILCgain value

disp('starting experiment...');
fprintf(ILR_Serial,'1'); % go to start menu 
% pause(0.01);
fprintf(ILR_Serial,'1'); % start experiment
disp('waiting for start frequency to settle...');
pause(10);

disp('start sweep')
for i = 1:(end_T-start_T)
    fprintf(ILR_Serial,'2'); % go to sample rate menu
    %pause(0.1);
    fprintf(ILR_Serial,num2str(end_T-i)); % set new sample rate
    pause(2);
end

disp('end sweep');
pause(2);

disp('ending experiment and closing connection');
fprintf(ILR_Serial,'1'); % go to start menu 
% pause(0.01);
fprintf(ILR_Serial,'3'); % start experiment
pause(1);

fclose(ILR_Serial); % close Serial Port
delete(ILR_Serial); % deleting Serial Port Object
clear ILR_Serial;