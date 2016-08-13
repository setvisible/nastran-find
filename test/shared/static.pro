# ============================================================
# SHARED TEST FILES
# ============================================================
# files that are to be copied from "$$PROJECT_DIR/test/shared/"
# to the build directory "$$BUILD_DIR/share/"
# ============================================================
#
include(../../nastranfind.pri)

DATA_DIRS = \
    broken \
    comment \
    cyclic \
    duplicate \
    first_last \
    multiline \
    quotes \
    singleline \
    subdirectory \
    symbolic_link


# DATA_DIRS += whitespace # temporaqry disabled


# TODO  unix {
# TODO      FILES += symbolic_links/link.dat
# TODO  }


# -------------------------------------------------------------
# Get all the files
# -------------------------------------------------------------
for(data_dir, DATA_DIRS) {
    files = $$files($$PWD/$$data_dir/*, true)
    win32:files ~= s|\\\\|/|g

    # Info.plist.in are handled below
    for(file, files) {
        !contains(file, ".*/Info\\.plist\\.in$") {
            !exists($$file/*):FILES += $$file
        }
    }
}


#OTHER_FILES += $$FILES


ABSOLUTE_TEST_SHARED_DIR = $$NF_SOURCE_TREE/test/shared/
defineReplace(stripTestShareDir) {
    win32 {
        !contains(1, ^.:.*):1 = $$OUT_PWD/$$1
    } else {
        !contains(1, ^/.*):1 = $$OUT_PWD/$$1
    }
    out = $$cleanPath($$1)
    out ~= s|^$$re_escape($$ABSOLUTE_TEST_SHARED_DIR)||$$i_flag
    return($$out)
}


# -------------------------------------------------------------
# conditionally deployed data
# -------------------------------------------------------------
!isEmpty(copydata) {
    copy2build.input = FILES
    copy2build.output = $$NF_DATA_PATH/${QMAKE_FUNC_FILE_IN_stripTestShareDir}
    isEmpty(vcproj):copy2build.variable_out = PRE_TARGETDEPS
    win32:copy2build.commands = $$QMAKE_COPY \"${QMAKE_FILE_IN}\" \"${QMAKE_FILE_OUT}\"
    unix:copy2build.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
    copy2build.name = COPY ${QMAKE_FILE_IN}
    copy2build.CONFIG += no_link
    QMAKE_EXTRA_COMPILERS += copy2build
}



