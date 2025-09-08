% Αρχικό σήμα φωνής
[signal,Fs] = audioread("Command.wav");
% Αναπαραγωγή του παλιού σήματος
soundsc(signal, Fs);
% Υπολογισμός ενέργειας ανά πλαίσιο
frameDuration = 0.025 ; %ισοδυναμεί με 5ms
frameLength = round(frameDuration * Fs);
frameStep = round(frameLength/2);
frames = floor((length(signal)-frameLength)/frameStep) + 1;
energy = zeros(frames,1); 
for i = 1:frames
    index = (i-1)*frameStep + 1;
    frame = signal(index:index+frameLength-1);
    energy(i) = sum(frame.^2);
end
% Ορισμός κατωφλίου ενέργειας για ανίχνευση της φωνής
thres = 0.01 * max(energy); 
% Δείκτες πλαισίων με φωνή
voicedFrames = find(energy>thres);
% Μετατροπή των πλαισίων σε δείκτες δείγματος
for i = 1:length(voicedFrames)
    index = (voicedFrames(i)-1)*frameStep + 1;
    ending = min(index + frameLength - 1,length(signal));
    samples(index:ending) = true;
end

% Νέο σήμα χωρίς παύσεις 
noPauseSignal = signal(samples);
% Σύγκριση με το κανονικό σήμα
t1 = (0:length(signal)-1) / Fs;
t2 = (0:length(noPauseSignal)-1) / Fs;
subplot(221);plot(t1,signal);title('Αρχικό Σήμα');xlabel('Χρόνος (sec)');ylabel('Πλάτος');
subplot(222);plot(t2,noPauseSignal);
title('Σήμα Χωρίς Παύσεις');xlabel('Χρόνος (sec)');ylabel('Πλάτος');
% Αναπαραγωγή του νέου σήματος
soundsc(noPauseSignal,  Fs);

