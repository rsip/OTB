# $Id$

ADD_SUBDIRECTORY(Common)
ADD_SUBDIRECTORY(IO)
ADD_SUBDIRECTORY(BasicFilters)
ADD_SUBDIRECTORY(FeatureExtraction)
ADD_SUBDIRECTORY(ChangeDetection)
ADD_SUBDIRECTORY(Learning)
ADD_SUBDIRECTORY(MultiScale)
ADD_SUBDIRECTORY(DisparityMap)
ADD_SUBDIRECTORY(SpatialReasoning)
ADD_SUBDIRECTORY(Projections)
ADD_SUBDIRECTORY(Radiometry)
ADD_SUBDIRECTORY(Fusion)
ADD_SUBDIRECTORY(Markov)
ADD_SUBDIRECTORY(SARPolarimetry)
ADD_SUBDIRECTORY(TestSystem)
ADD_SUBDIRECTORY(OBIA)
ADD_SUBDIRECTORY(ObjectDetection)
ADD_SUBDIRECTORY(Fuzzy)
ADD_SUBDIRECTORY(MultiTemporal)




IF(OTB_USE_VISU_GUI)
        ADD_SUBDIRECTORY(Visu)
        ADD_SUBDIRECTORY(Gui)
        ADD_SUBDIRECTORY(Visualization)
ENDIF(OTB_USE_VISU_GUI)

IF(OTB_USE_PQXX)
        ADD_SUBDIRECTORY(GeospatialAnalysis)
ENDIF(OTB_USE_PQXX)

