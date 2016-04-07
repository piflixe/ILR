% script to control sweep experiments
%
% author: Felix Piela
% dec 2015
%

% Arduino needs to be configured so that Serial Console doesn't return
% anything. (Proper Messages over Serial still in progress 
% see https://github.com/piflixe/ILR/issues/5 

%% HEADER
sampleFreq_start = 29000;
sampleFreq_end = 31000;

%% SET UP
ILR_Serial = serial('COM8'); % Create Serial Port
set (ILR_Serial, ...  % Set Serial Port Properties
    'BaudRate',115200, ... % set BAUD rate
    'terminator','LF' ... % set end of message to Line Feed (Arduino)
);
fopen(ILR_Serial); % Open Serial Port
pause(0.3); % seems to need some time to settle
pause(2); % wait for system to settle on possible distortion after activiation

%% INIT ARDUINO
fprintf(ILR_Serial,'2'); % go to sampleFreq menu
pause(0.1);
fprintf(ILR_Serial,num2str(sampleFreq_start)); % set to start frequency
pause(0.1);
fprintf(ILR_Serial,'1');
pause(0.1);
fprintf(ILR_Serial,'3'); % stop experiment since it is started right after setting frequency
pause(0.1);

fprintf(ILR_Serial,'3'); % go to Ki menu % function not implemented
pause(0.1);
fprintf(ILR_Serial,'30'); % set Ki value
pause(0.1);

fprintf(ILR_Serial,'4'); % go to Kp menu % function not implemented
pause(0.1);
fprintf(ILR_Serial,'99'); % set Kp value
pause(0.1);

fprintf(ILR_Serial,'5'); % go to PhaseLead menu % function not implemented
pause(0.1);
fprintf(ILR_Serial,'6'); % set PhaseLead
pause(0.1);

fprintf(ILR_Serial,'6'); % go to Nsmooth menu % function not implemented
pause(0.1);
fprintf(ILR_Serial,'10'); % set smoothing width
pause(0.1);


%%
disp('starting experiment...');
fprintf(ILR_Serial,'1'); % go to start menu 
pause(0.01);
fprintf(ILR_Serial,'1'); % start experiment
disp('waiting for start frequency to settle...');
pause(1);

%% SWEEP
disp('start sweep')
for i = 0:10:(sampleFreq_end-sampleFreq_start)
    fprintf(ILR_Serial,'2'); % go to sample rate menu
    pause(0.05);
    fprintf(ILR_Serial,num2str(sampleFreq_start + i)); % set new sample rate
    %disp('current sample frequency: ');
    disp(sampleFreq_start + i);
    pause(0.05);
end

disp('end sweep');
pause(2);

%%  
disp('ending experiment and closing connection');
fprintf(ILR_Serial,'1'); % go to start menu 
pause(0.01);
fprintf(ILR_Serial,'3'); % stop experiment
pause(1);


%% clear session
fclose(ILR_Serial); % close Serial Port
delete(ILR_Serial); % deleting Serial Port Object
clear ILR_Serial;
