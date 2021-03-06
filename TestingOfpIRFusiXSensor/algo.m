function [StO2] = algo(wavelength_U, wavelength_W)
%ALGO Run the gateway algorithm simulation
incidentIntensity = 2;
previousStO2 = 0.60;
for i = 2:size(wavelength_U)
    oDLast_U = 0;
    oDCurrent_U = 0;
    if wavelength_U(i-1) ~= 0
        oDLast_U = log10(incidentIntensity / wavelength_U(i - 1));
    end
    if wavelength_U(i) ~= 0
        oDCurrent_U = log10(incidentIntensity / wavelength_U(i));
    end
    oDChange_U = oDCurrent_U - oDLast_U;

    oDLast_W = 0;
    oDCurrent_W = 0;
    if wavelength_W(i-1) ~= 0
        oDLast_W = log10(incidentIntensity / wavelength_W(i - 1));
    end
    if wavelength_W(i) ~= 0
        oDCurrent_W = log10(incidentIntensity / wavelength_W(i));
    end
    oDChange_W = oDCurrent_W - oDLast_W;


    if oDChange_U == 0 && oDChange_W == 0
        StO2(i - 1) = previousStO2;
    else
        frontCoeff = 1.0 / 0.0125;
        oDMatrix = [oDChange_U; oDChange_W];
        inputMatrix = [0.38*4.38, 0.14*4.38; 0.18*3.94, 0.30*3.94];
        result = inputMatrix\oDMatrix .* frontCoeff;
        concHb = result(1,1);
        concHbO2 = result(2, 1);

        StO2(i - 1) = concHbO2 / (concHb + concHbO2);
        previousStO2 = StO2(i-1);
    end
end
StO2(i) = previousStO2;
end