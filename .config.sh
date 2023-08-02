source /home/oleksii/soft/Root/install_root_6.24/bin/thisroot.sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/oleksii/soft/QnTools/install/lib/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/oleksii/cbmdir/flow_drawing_tools/build
export QnTools_DIR=/home/oleksii/soft/QnTools/install/lib/cmake/QnTools

alias flowdrawconfig_mj_discriminator_on='cd /home/oleksii/cbmdir/flow_drawing_tools/build && cmake -DDISCRIMINATOR_MODE=ON ../ && mj && cd -'
alias flowdrawconfig_mj_discriminator_off='cd /home/oleksii/cbmdir/flow_drawing_tools/build && cmake -DDISCRIMINATOR_MODE=OFF ../ && mj && cd -'
