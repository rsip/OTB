WRAP_CLASS("itk::ScaleTransform" POINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("${ITKM_D}${d}" "${ITKT_D},${d}")
  ENDFOREACH(d)
END_WRAP_CLASS()
