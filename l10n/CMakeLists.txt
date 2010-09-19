find_package(Qt4 REQUIRED)

qt4_add_translation(HeyTrack_QM
    cs_CZ.ts
)

add_custom_target(HeyTrackLocalization ALL DEPENDS ${HeyTrack_QM})

install(FILES ${HeyTrack_QM} DESTINATION ${HEYTRACK_TRANSLATION_DIR})
