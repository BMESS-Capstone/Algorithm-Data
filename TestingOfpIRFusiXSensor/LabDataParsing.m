T = readtable("pIRfusiX Lab Testing_OcclusionData_ExcelFile.xlsx");
U = table2array(T(:, "U")); %760 --> Hb
W = table2array(T(:, "W")); %860 --> HbO2

%looking at the data, we have a 2Hz sampling time
time = []; 
x = 0;
for i = 1:1377
    time(end+1) = i*0.5;
end

figure; 
plot(time,U, '*r'); 
hold on
plot(time,W, '*blu'); 
title("Raw Spectral Intensity Recieved by pIRfusiX Sensor during Occlusion Testing");
xlabel("seconds");
xline(130, 'red', "*");
xline(420, 'green', "*");
legend("760nm (Hhb)", "860nm (O2Hb)", "Occlusion", "Reperfusion"); 


%now we plot the commercical sensor
time1 = []; 
x = 0;%10Hz sample rate
for i = 1:7412
    time1(end+1) = i*0.1;
end 
T1 = readtable('Commercial_sensor_ExcelFormat.xls');
Hhb = table2array(T1(:, 4));
Hhb(1,1) = 0;
O2Hb = table2array(T1(:,5)); 
O2Hb(1,1) = 0;

figure; 
plot(time1,Hhb, 'blu'); hold on; plot(time1,O2Hb, 'red');
title("Laboratory NIRS Sensor (PortaMon)");
xlabel('seconds');
ylabel('relative concentration changes')
xline(130, '--r');
xline(410, '--g');
legend("Hhb (DeOxy)", "O2Hb (Oxy)", 'Occlusion (280mmHg)', 'Reperfusion');

%lowpass filtering
num = 1/10*[1 1 1 1 1 1 1 1 1 1];
den = [1];
U_filt = filter(num,den,U);
W_filt = filter(num, den, W); 



figure; 
plot(time,U_filt, 'r'); 
hold on
plot(time,W_filt, 'blu'); 
title("Filtered Spectral Intensity Recieved by pIRfusiX Sensor during Occlusion Testing");
xlabel("seconds");
ylabel("microW/cm^2");
xline(130, '--r');
xline(410, '--g');
legend("760nm (Hhb)", "860nm (O2Hb)", "Occlusion (280mmHg)", "Reperfusion"); 

%calculate StO2
%StO2 is defined as the change in perfusion 
%take the difference of all measurements then divide .... Doesnt
%work?????????????????????
delta_Hhb = checkFrameDiff(U_filt); 
delta_O2Hb = checkFrameDiff(W_filt);

delta_HhbCom = checkFrameDiff(Hhb);
delta_O2HbCom = checkFrameDiff(O2Hb); 


StO2 = delta_O2Hb./(delta_Hhb + delta_O2Hb);


