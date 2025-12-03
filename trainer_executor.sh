mkdir experiments

# CM 0
./bin/trainer imgs/int20040906_00421872_img0_u16be-1x4200x2154.raw 0 0.001 1 > experiments/INT_0_0-001.txt
./bin/trainer imgs/int20040906_00421872_img0_u16be-1x4200x2154.raw 0 0.01 300 > experiments/INT_0_0-01.txt
./bin/trainer imgs/int20040906_00421872_img0_u16be-1x4200x2154.raw 0 0.1 300 > experiments/INT_0_0-1.txt

./bin/trainer imgs/0004692896-20240825-OSIRIS-OsirisBroadBandImage_u16be-1x2056x2073.raw 0 0.001 300 > experiments/GTC_0_0-001.txt
./bin/trainer imgs/0004692896-20240825-OSIRIS-OsirisBroadBandImage_u16be-1x2056x2073.raw 0 0.01 300 > experiments/GTC_0_0-01.txt
./bin/trainer imgs/0004692896-20240825-OSIRIS-OsirisBroadBandImage_u16be-1x2056x2073.raw 0 0.1 300 > experiments/GTC_0_0-1.txt

./bin/trainer imgs/jkt19990925_00100583_img0-u16be-1x2148x2148.raw 0 0.001 300 > experiments/JKT_0_0-001.txt
./bin/trainer imgs/jkt19990925_00100583_img0-u16be-1x2148x2148.raw 0 0.01 300 > experiments/JKT_0_0-01.txt
./bin/trainer imgs/jkt19990925_00100583_img0-u16be-1x2148x2148.raw 0 0.1 300 > experiments/JKT_0_0-1.txt

./bin/trainer imgs/wht20120123_01754958_img0-u16be-1x2501x2148.raw 0 0.001 300 > experiments/WHT_0_0-001.txt
./bin/trainer imgs/wht20120123_01754958_img0-u16be-1x2501x2148.raw 0 0.01 300 > experiments/WHT_0_0-01.txt
./bin/trainer imgs/wht20120123_01754958_img0-u16be-1x2501x2148.raw 0 0.1 300 > experiments/WHT_0_0-1.txt

./bin/trainer imgs/coj0m403-kb24-20220213-0274-e00_img0-u16be-1x2112x3136.raw 0 0.001 300 > experiments/LCO_0_0-001.txt
./bin/trainer imgs/coj0m403-kb24-20220213-0274-e00_img0-u16be-1x2112x3136.raw 0 0.01 300 > experiments/LCO_0_0-01.txt
./bin/trainer imgs/coj0m403-kb24-20220213-0274-e00_img0-u16be-1x2112x3136.raw 0 0.1 300 > experiments/LCO_0_0-1.txt

./bin/trainer imgs/ERIS.2024-01-01T06:15:12.592_u16be-1x2046x2048.raw 0 0.001 300 > experiments/VLT_0_0-001.txt
./bin/trainer imgs/ERIS.2024-01-01T06:15:12.592_u16be-1x2046x2048.raw 0 0.01 300 > experiments/VLT_0_0-01.txt
./bin/trainer imgs/ERIS.2024-01-01T06:15:12.592_u16be-1x2046x2048.raw 0 0.1 300 > experiments/VLT_0_0-1.txt

./bin/trainer imgs/TJO2459273.28051_V_imr_img0-u16be-1x4108x4096.raw 0 0.001 300 > experiments/TJO_0_0-001.txt
./bin/trainer imgs/TJO2459273.28051_V_imr_img0-u16be-1x4108x4096.raw 0 0.01 300 > experiments/TJO_0_0-01.txt
./bin/trainer imgs/TJO2459273.28051_V_imr_img0-u16be-1x4108x4096.raw 0 0.1 300 > experiments/TJO_0_0-1.txt

# CM 1
./bin/trainer imgs/int20040906_00421872_img0_u16be-1x4200x2154.raw 1 0.001 300 > experiments/INT_1_0-001.txt
./bin/trainer imgs/int20040906_00421872_img0_u16be-1x4200x2154.raw 1 0.01 300 > experiments/INT_1_0-01.txt
./bin/trainer imgs/int20040906_00421872_img0_u16be-1x4200x2154.raw 1 0.1 300 > experiments/INT_1_0-1.txt

./bin/trainer imgs/0004692896-20240825-OSIRIS-OsirisBroadBandImage_u16be-1x2056x2073.raw 1 0.001 300 > experiments/GTC_1_0-001.txt
./bin/trainer imgs/0004692896-20240825-OSIRIS-OsirisBroadBandImage_u16be-1x2056x2073.raw 1 0.01 300 > experiments/GTC_1_0-01.txt
./bin/trainer imgs/0004692896-20240825-OSIRIS-OsirisBroadBandImage_u16be-1x2056x2073.raw 1 0.1 300 > experiments/GTC_1_0-1.txt

./bin/trainer imgs/jkt19990925_00100583_img0-u16be-1x2148x2148.raw 1 0.001 300 > experiments/JKT_1_0-001.txt
./bin/trainer imgs/jkt19990925_00100583_img0-u16be-1x2148x2148.raw 1 0.01 300 > experiments/JKT_1_0-01.txt
./bin/trainer imgs/jkt19990925_00100583_img0-u16be-1x2148x2148.raw 1 0.1 300 > experiments/JKT_1_0-1.txt

./bin/trainer imgs/wht20120123_01754958_img0-u16be-1x2501x2148.raw 1 0.001 300 > experiments/WHT_1_0-001.txt
./bin/trainer imgs/wht20120123_01754958_img0-u16be-1x2501x2148.raw 1 0.01 300 > experiments/WHT_1_0-01.txt
./bin/trainer imgs/wht20120123_01754958_img0-u16be-1x2501x2148.raw 1 0.1 300 > experiments/WHT_1_0-1.txt

./bin/trainer imgs/coj0m403-kb24-20220213-0274-e00_img0-u16be-1x2112x3136.raw 1 0.001 300 > experiments/LCO_1_0-001.txt
./bin/trainer imgs/coj0m403-kb24-20220213-0274-e00_img0-u16be-1x2112x3136.raw 1 0.01 300 > experiments/LCO_1_0-01.txt
./bin/trainer imgs/coj0m403-kb24-20220213-0274-e00_img0-u16be-1x2112x3136.raw 1 0.1 300 > experiments/LCO_1_0-1.txt

./bin/trainer imgs/ERIS.2024-01-01T06:15:12.592_u16be-1x2046x2048.raw 1 0.001 300 > experiments/VLT_1_0-001.txt
./bin/trainer imgs/ERIS.2024-01-01T06:15:12.592_u16be-1x2046x2048.raw 1 0.01 300 > experiments/VLT_1_0-01.txt
./bin/trainer imgs/ERIS.2024-01-01T06:15:12.592_u16be-1x2046x2048.raw 1 0.1 300 > experiments/VLT_1_0-1.txt

./bin/trainer imgs/TJO2459273.28051_V_imr_img0-u16be-1x4108x4096.raw 1 0.001 300 > experiments/TJO_1_0-001.txt
./bin/trainer imgs/TJO2459273.28051_V_imr_img0-u16be-1x4108x4096.raw 1 0.01 300 > experiments/TJO_1_0-01.txt
./bin/trainer imgs/TJO2459273.28051_V_imr_img0-u16be-1x4108x4096.raw 1 0.1 300 > experiments/TJO_1_0-1.txt

# CM 2
./bin/trainer imgs/int20040906_00421872_img0_u16be-1x4200x2154.raw 2 0.001 300 > experiments/INT_2_0-001.txt
./bin/trainer imgs/int20040906_00421872_img0_u16be-1x4200x2154.raw 2 0.01 300 > experiments/INT_2_0-01.txt
./bin/trainer imgs/int20040906_00421872_img0_u16be-1x4200x2154.raw 2 0.1 300 > experiments/INT_2_0-1.txt

./bin/trainer imgs/0004692896-20240825-OSIRIS-OsirisBroadBandImage_u16be-1x2056x2073.raw 2 0.001 300 > experiments/GTC_2_0-001.txt
./bin/trainer imgs/0004692896-20240825-OSIRIS-OsirisBroadBandImage_u16be-1x2056x2073.raw 2 0.01 300 > experiments/GTC_2_0-01.txt
./bin/trainer imgs/0004692896-20240825-OSIRIS-OsirisBroadBandImage_u16be-1x2056x2073.raw 2 0.1 300 > experiments/GTC_2_0-1.txt

./bin/trainer imgs/jkt19990925_00100583_img0-u16be-1x2148x2148.raw 2 0.001 300 > experiments/JKT_2_0-001.txt
./bin/trainer imgs/jkt19990925_00100583_img0-u16be-1x2148x2148.raw 2 0.01 300 > experiments/JKT_2_0-01.txt
./bin/trainer imgs/jkt19990925_00100583_img0-u16be-1x2148x2148.raw 2 0.1 300 > experiments/JKT_2_0-1.txt

./bin/trainer imgs/wht20120123_01754958_img0-u16be-1x2501x2148.raw 2 0.001 300 > experiments/WHT_2_0-001.txt
./bin/trainer imgs/wht20120123_01754958_img0-u16be-1x2501x2148.raw 2 0.01 300 > experiments/WHT_2_0-01.txt
./bin/trainer imgs/wht20120123_01754958_img0-u16be-1x2501x2148.raw 2 0.1 300 > experiments/WHT_2_0-1.txt

./bin/trainer imgs/coj0m403-kb24-20220213-0274-e00_img0-u16be-1x2112x3136.raw 2 0.001 300 > experiments/LCO_2_0-001.txt
./bin/trainer imgs/coj0m403-kb24-20220213-0274-e00_img0-u16be-1x2112x3136.raw 2 0.01 300 > experiments/LCO_2_0-01.txt
./bin/trainer imgs/coj0m403-kb24-20220213-0274-e00_img0-u16be-1x2112x3136.raw 2 0.1 300 > experiments/LCO_2_0-1.txt

./bin/trainer imgs/ERIS.2024-01-01T06:15:12.592_u16be-1x2046x2048.raw 2 0.001 300 > experiments/VLT_2_0-001.txt
./bin/trainer imgs/ERIS.2024-01-01T06:15:12.592_u16be-1x2046x2048.raw 2 0.01 300 > experiments/VLT_2_0-01.txt
./bin/trainer imgs/ERIS.2024-01-01T06:15:12.592_u16be-1x2046x2048.raw 2 0.1 300 > experiments/VLT_2_0-1.txt

./bin/trainer imgs/TJO2459273.28051_V_imr_img0-u16be-1x4108x4096.raw 2 0.001 300 > experiments/TJO_2_0-001.txt
./bin/trainer imgs/TJO2459273.28051_V_imr_img0-u16be-1x4108x4096.raw 2 0.01 300 > experiments/TJO_2_0-01.txt
./bin/trainer imgs/TJO2459273.28051_V_imr_img0-u16be-1x4108x4096.raw 2 0.1 300 > experiments/TJO_2_0-1.txt

# CM 3
./bin/trainer imgs/int20040906_00421872_img0_u16be-1x4200x2154.raw 3 0.001 300 > experiments/INT_3_0-001.txt
./bin/trainer imgs/int20040906_00421872_img0_u16be-1x4200x2154.raw 3 0.01 300 > experiments/INT_3_0-01.txt
./bin/trainer imgs/int20040906_00421872_img0_u16be-1x4200x2154.raw 3 0.1 300 > experiments/INT_3_0-1.txt

./bin/trainer imgs/0004692896-20240825-OSIRIS-OsirisBroadBandImage_u16be-1x2056x2073.raw 3 0.001 300 > experiments/GTC_3_0-001.txt
./bin/trainer imgs/0004692896-20240825-OSIRIS-OsirisBroadBandImage_u16be-1x2056x2073.raw 3 0.01 300 > experiments/GTC_3_0-01.txt
./bin/trainer imgs/0004692896-20240825-OSIRIS-OsirisBroadBandImage_u16be-1x2056x2073.raw 3 0.1 300 > experiments/GTC_3_0-1.txt

./bin/trainer imgs/jkt19990925_00100583_img0-u16be-1x2148x2148.raw 3 0.001 300 > experiments/JKT_3_0-001.txt
./bin/trainer imgs/jkt19990925_00100583_img0-u16be-1x2148x2148.raw 3 0.01 300 > experiments/JKT_3_0-01.txt
./bin/trainer imgs/jkt19990925_00100583_img0-u16be-1x2148x2148.raw 3 0.1 300 > experiments/JKT_3_0-1.txt

./bin/trainer imgs/wht20120123_01754958_img0-u16be-1x2501x2148.raw 3 0.001 300 > experiments/WHT_3_0-001.txt
./bin/trainer imgs/wht20120123_01754958_img0-u16be-1x2501x2148.raw 3 0.01 300 > experiments/WHT_3_0-01.txt
./bin/trainer imgs/wht20120123_01754958_img0-u16be-1x2501x2148.raw 3 0.1 300 > experiments/WHT_3_0-1.txt

./bin/trainer imgs/coj0m403-kb24-20220213-0274-e00_img0-u16be-1x2112x3136.raw 3 0.001 300 > experiments/LCO_3_0-001.txt
./bin/trainer imgs/coj0m403-kb24-20220213-0274-e00_img0-u16be-1x2112x3136.raw 3 0.01 300 > experiments/LCO_3_0-01.txt
./bin/trainer imgs/coj0m403-kb24-20220213-0274-e00_img0-u16be-1x2112x3136.raw 3 0.1 300 > experiments/LCO_3_0-1.txt

./bin/trainer imgs/ERIS.2024-01-01T06:15:12.592_u16be-1x2046x2048.raw 3 0.001 300 > experiments/VLT_3_0-001.txt
./bin/trainer imgs/ERIS.2024-01-01T06:15:12.592_u16be-1x2046x2048.raw 3 0.01 300 > experiments/VLT_3_0-01.txt
./bin/trainer imgs/ERIS.2024-01-01T06:15:12.592_u16be-1x2046x2048.raw 3 0.1 300 > experiments/VLT_3_0-1.txt

./bin/trainer imgs/TJO2459273.28051_V_imr_img0-u16be-1x4108x4096.raw 3 0.001 300 > experiments/TJO_3_0-001.txt
./bin/trainer imgs/TJO2459273.28051_V_imr_img0-u16be-1x4108x4096.raw 3 0.01 300 > experiments/TJO_3_0-01.txt
./bin/trainer imgs/TJO2459273.28051_V_imr_img0-u16be-1x4108x4096.raw 3 0.1 300 > experiments/TJO_3_0-1.txt

# CM 4
./bin/trainer imgs/int20040906_00421872_img0_u16be-1x4200x2154.raw 4 0.001 300 > experiments/INT_4_0-001.txt
./bin/trainer imgs/int20040906_00421872_img0_u16be-1x4200x2154.raw 4 0.01 300 > experiments/INT_4_0-01.txt
./bin/trainer imgs/int20040906_00421872_img0_u16be-1x4200x2154.raw 4 0.1 300 > experiments/INT_4_0-1.txt

./bin/trainer imgs/0004692896-20240825-OSIRIS-OsirisBroadBandImage_u16be-1x2056x2073.raw 4 0.001 300 > experiments/GTC_4_0-001.txt
./bin/trainer imgs/0004692896-20240825-OSIRIS-OsirisBroadBandImage_u16be-1x2056x2073.raw 4 0.01 300 > experiments/GTC_4_0-01.txt
./bin/trainer imgs/0004692896-20240825-OSIRIS-OsirisBroadBandImage_u16be-1x2056x2073.raw 4 0.1 300 > experiments/GTC_4_0-1.txt

./bin/trainer imgs/jkt19990925_00100583_img0-u16be-1x2148x2148.raw 4 0.001 300 > experiments/JKT_4_0-001.txt
./bin/trainer imgs/jkt19990925_00100583_img0-u16be-1x2148x2148.raw 4 0.01 300 > experiments/JKT_4_0-01.txt
./bin/trainer imgs/jkt19990925_00100583_img0-u16be-1x2148x2148.raw 4 0.1 300 > experiments/JKT_4_0-1.txt

./bin/trainer imgs/wht20120123_01754958_img0-u16be-1x2501x2148.raw 4 0.001 300 > experiments/WHT_4_0-001.txt
./bin/trainer imgs/wht20120123_01754958_img0-u16be-1x2501x2148.raw 4 0.01 300 > experiments/WHT_4_0-01.txt
./bin/trainer imgs/wht20120123_01754958_img0-u16be-1x2501x2148.raw 4 0.1 300 > experiments/WHT_4_0-1.txt

./bin/trainer imgs/coj0m403-kb24-20220213-0274-e00_img0-u16be-1x2112x3136.raw 4 0.001 300 > experiments/LCO_4_0-001.txt
./bin/trainer imgs/coj0m403-kb24-20220213-0274-e00_img0-u16be-1x2112x3136.raw 4 0.01 300 > experiments/LCO_4_0-01.txt
./bin/trainer imgs/coj0m403-kb24-20220213-0274-e00_img0-u16be-1x2112x3136.raw 4 0.1 300 > experiments/LCO_4_0-1.txt

./bin/trainer imgs/ERIS.2024-01-01T06:15:12.592_u16be-1x2046x2048.raw 4 0.001 300 > experiments/VLT_4_0-001.txt
./bin/trainer imgs/ERIS.2024-01-01T06:15:12.592_u16be-1x2046x2048.raw 4 0.01 300 > experiments/VLT_4_0-01.txt
./bin/trainer imgs/ERIS.2024-01-01T06:15:12.592_u16be-1x2046x2048.raw 4 0.1 300 > experiments/VLT_4_0-1.txt

./bin/trainer imgs/TJO2459273.28051_V_imr_img0-u16be-1x4108x4096.raw 4 0.001 300 > experiments/TJO_4_0-001.txt
./bin/trainer imgs/TJO2459273.28051_V_imr_img0-u16be-1x4108x4096.raw 4 0.01 300 > experiments/TJO_4_0-01.txt
./bin/trainer imgs/TJO2459273.28051_V_imr_img0-u16be-1x4108x4096.raw 4 0.1 300 > experiments/TJO_4_0-1.txt