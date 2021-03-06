PROJECT(OBIAExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(ImageToLabelToImage ImageToLabelToImage.cxx )
TARGET_LINK_LIBRARIES(ImageToLabelToImage OTBCommon OTBIO ${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES})

ADD_EXECUTABLE(ShapeAttributeComputation ShapeAttributeComputation.cxx )
TARGET_LINK_LIBRARIES(ShapeAttributeComputation OTBCommon OTBIO ${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES})

ADD_EXECUTABLE(KeepNObjects KeepNObjects.cxx )
TARGET_LINK_LIBRARIES(KeepNObjects OTBCommon OTBIO ${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES})

ADD_EXECUTABLE(RadiometricAttributesLabelMapFilterExample RadiometricAttributesLabelMapFilterExample.cxx )
TARGET_LINK_LIBRARIES(RadiometricAttributesLabelMapFilterExample OTBCommon OTBIO OTBBasicFilters OTBSpatialReasoning  OTBFeatureExtraction OTBLearning ${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES})

ADD_EXECUTABLE(LabelMapToVectorData LabelMapToVectorData.cxx )
TARGET_LINK_LIBRARIES(LabelMapToVectorData OTBCommon OTBIO OTBBasicFilters ${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES})

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/OBIA)
SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)

#Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(INPUTLARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbOBIAExamplesTests)

SET(TOL 0.0)
SET(EPS 0.001)

ADD_TEST( trTeOBIAImageToLabelToImageTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/OBIAI2L2IOutput.tif
                                        ${TEMP}/OBIAI2L2IOutput.tif
        OBIAImageToLabelToImageTest
        ${INPUTDATA}/MSLabeledOutput.tif
        ${TEMP}/OBIAI2L2IOutput.tif
	${TEMP}/OBIAI2L2IInPretty.png
	${TEMP}/OBIAI2L2IOutPretty.png
	1 1108 0
        )

ADD_TEST(trTeOBIAShapeAttributeComputationTest ${EXE_TESTS}
	--compare-ascii ${EPS}	${BASELINE}/OBIAShapeAttribute.txt
	                        ${TEMP}/OBIAShapeAttribute.txt
	OBIAShapeAttributeComputationTest
	${INPUTDATA}/MSLabeledOutput.tif
	${TEMP}/OBIAShapeAttribute.txt
)

ADD_TEST(trTeOBIARadiometricAttributesLabelMapFilterTest ${EXE_TESTS}
	--compare-image ${TOL}	${BASELINE}/OBIARadiometricAttributes.tif
	                        ${TEMP}/OBIARadiometricAttributes.tif
	RadiometricAttributesLabelMapFilterExample
	${INPUTDATA}/qb_RoadExtract.tif 
	${TEMP}/OBIARadiometricAttributes.tif
	${TEMP}/qb_RoadExtract_Radiometry_pretty.png
	STATS::Band1::Mean
	0
	0.5
	16
	16
	50
	1.0
)
    
ADD_TEST(trTeLabelMapToVectorData ${EXE_TESTS}
	LabelMapToVectorDataTest
        ${INPUTDATA}/rcc8_mire1.png
        ${TEMP}/rcc8_mire2_vectorizer.shp
)
#need to add the test of radiometric labelmap filter
  
INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbOBIAExamplesTests otbOBIAExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbOBIAExamplesTests  ITKAlgorithms ITKStatistics ITKNumerics OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBFusion OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
