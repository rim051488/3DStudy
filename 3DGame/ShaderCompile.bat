set FXCOMPILER=fxc.exe

%FXCOMPILER% /T vs_5_0 /E "main" /O3  /Fo s.vso Shader/Skinning.hlsl
%FXCOMPILER% /T vs_5_0 /E "main" /O3  /Fo ns4.vso Shader/NormS4VS.hlsl
pause