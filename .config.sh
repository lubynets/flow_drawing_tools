source /home/oleksii/soft/Root/install_root_6.24/bin/thisroot.sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/oleksii/soft/QnTools/install/lib
INSTALL_DIR=${CMAKE_INSTALL_PREFIX}
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/oleksii/cbmdir/flow_drawing_tools/install/lib
export PATH=$PATH:/home/oleksii/cbmdir/flow_drawing_tools/install/bin
export QnTools_DIR=/home/oleksii/soft/QnTools/install/lib/cmake/QnTools

alias fdt_mji_discriminator_on='cd /home/oleksii/cbmdir/flow_drawing_tools/build && cmake -DDISCRIMINATOR_MODE=ON ../ && mji ; cd /home/oleksii/cbmdir/flow_drawing_tools/install/bin'
alias fdt_mji_discriminator_off='cd /home/oleksii/cbmdir/flow_drawing_tools/build && cmake -DDISCRIMINATOR_MODE=OFF ../ && mji ; cd /home/oleksii/cbmdir/flow_drawing_tools/install/bin'
