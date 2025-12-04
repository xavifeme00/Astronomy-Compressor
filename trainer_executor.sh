#!/bin/bash

mkdir -p experiments

imgs=(
    "INT imgs/int20040906_00421872_img0_u16be-1x4200x2154.raw"
    "GTC imgs/0004692896-20240825-OSIRIS-OsirisBroadBandImage_u16be-1x2056x2073.raw"
    "JKT imgs/jkt19990925_00100583_img0-u16be-1x2148x2148.raw"
    "WHT imgs/wht20120123_01754958_img0-u16be-1x2501x2148.raw"
    "LCO imgs/coj0m403-kb24-20220213-0274-e00_img0-u16be-1x2112x3136.raw"
    "VLT imgs/ERIS.2024-01-01T06:15:12.592_u16be-1x2046x2048.raw"
    "TJO imgs/TJO2459273.28051_V_imr_img0-u16be-1x4108x4096.raw"
)

cms=(4 3 2 1 0)

rls=(0.1 0.01 0.005 0.001)

eps=(0.1 0.01 0.001 0.0001)

epochs=300

echo "== Llençant GRID SEARCH =="

for cm in "${cms[@]}"; do
    for entry in "${imgs[@]}"; do
        for rl in "${rls[@]}"; do
            for ep in "${eps[@]}"; do
                read -r name path <<< "$entry"

                out="experiments/${name}_${cm}_${rl}.txt"

                echo "Executant: $name CM=$cm RL=$rl EP=$ep"
                ./bin/trainer "$path" "$cm" "$rl" "$ep" "$epochs" > "$out"
            done
        done
    done
done