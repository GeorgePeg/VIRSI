% Παράδειγμα σήματος φωνής
[signal,Fs] = audioread("Command.wav");
% Μεταβλητές του συμπιεστή 
threshold_dB = -10;
ratio = 4; 
attackTime = 0.1;
releaseTime = 0.45;
makeUpGain_dB = 4;
% Μετατροπή του κατωφλίου στην γραμμική κλίμακα
thres = 10^(threshold_dB/20);
makeUpGain = 10^(makeUpGain_dB/20);
% Υπολογισμός RMS για κάθε πλαίσιο
frame_N = round(Fs*0.01); % Κάθε πλαίσιο έχει μέγεθος 10ms
rms = sqrt(movmean(signal.^2 , frame_N));
% Εφαρμογή της συμπίεσης 
gain = ones(size(rms));
for i = 1:length(rms)
    if rms(i) > thres
        gain(i) = (rms(i)/thres)^((1-ratio)/ratio);
    end
end
% Ορισμοί των smoothing factors 
a1 = exp(-1/(Fs*attackTime));
a2 = exp(-1/(Fs*releaseTime));
new_gain = gain;
for i = 2:length(gain)
    if gain(i) < new_gain(i-1)
        new_gain(i) = a1 * new_gain(i-1) + (1-a1)*gain(i);
    else
        new_gain(i) = a2 * new_gain(i-1) + (1-a2)*gain(i);
    end
end
% Τελική ενίσχυση 
new_signal = signal .* new_gain * makeUpGain;
% Απεικόνιση γραφημάτων
t = (0:length(signal)-1) / Fs;
figure;
subplot(3,1,1); plot(t,signal);title('Σήμα');ylim([-3 3]);
subplot(3,1,2); plot(t, new_gain); title('Gain Evenlope');
subplot(3,1,3);plot(t,new_signal);title('Συμπιεσμένο Σήμα'); ylim([-3 3]);
% Αναπαραγωγή του συμπιεσμένου σήματος
soundsc(new_signal,Fs);
