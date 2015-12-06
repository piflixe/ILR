% script to control sweep experiments
%
% author: Felix Piela
% dec 2015
%

% Arduino needs to be configured so that Serial Console doesn't return
% anything. (Proper Messages over Serial still in progress, see, 

ILR_Serial = serial('COM8'); % Create Serial Port
set (ILR_Serial, ...  % Set Serial Port Properties
    'BaudRate',115200, ... % set BAUD rate
    'terminator','LF' ... % set end of message to Line Feed (Arduino)
);

fopen(ILR_Serial); % Open Serial Port
pause(0.3); % seems to need some time to settle
fprintf(ILR_Serial,'1'); % go to start menu 
% pause(0.01);
fprintf(ILR_Serial,'1'); % start experiment
% pause(0.01);
for i = 1:150
    fprintf(ILR_Serial,'2'); % go to sample rate menu
    %pause(0.1);
    fprintf(ILR_Serial,num2str(100+i)); % set new sample rate
    pause(0.15);
end

fclose(ILR_Serial); % close Serial Port
delete(ILR_Serial); % deleting Serial Port Object
clear ILR_Serial;