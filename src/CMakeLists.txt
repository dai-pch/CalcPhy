
AUX_SOURCE_DIRECTORY(. DIR_CalcPhy_DIR)
ADD_EXECUTABLE( CalcPhy ${DIR_CalcPhy_DIR} )
target_link_libraries( CalcPhy number )
