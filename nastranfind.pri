!isEmpty(NF_PRI_INCLUDED):error("nastranfind.pri already included")
NF_PRI_INCLUDED = 1

include($$PWD/version.pri)

#-------------------------------------------------
# Functions copied from Qt Creator
#-------------------------------------------------
isEqual(QT_MAJOR_VERSION, 5) {
    
    defineReplace(cleanPath) {
        return($$clean_path($$1))
    }
    
    defineReplace(targetPath) {
        return($$shell_path($$1))
    }
    
} else { # qt5
    
    defineReplace(cleanPath) {
        win32:1 ~= s|\\\\|/|g
        contains(1, ^/.*):pfx = /
        else:pfx =
        segs = $$split(1, /)
        out =
        for(seg, segs) {
            equals(seg, ..):out = $$member(out, 0, -2)
            else:!equals(seg, .):out += $$seg
        }
        return($$join(out, /, $$pfx))
    }
    
    defineReplace(targetPath) {
        return($$replace(1, /, $$QMAKE_DIR_SEP))
    }
    
} # qt5


#isEqual(QT_MAJOR_VERSION, 5) {
#
#    # For use in custom compilers which just copy files
#    defineReplace(stripSrcDir) {
#        return($$relative_path($$absolute_path($$1, $$OUT_PWD), $$_PRO_FILE_PWD_))
#    }
#
#} else { # qt5
#
#    # For use in custom compilers which just copy files
#    win32:i_flag = i
#    defineReplace(stripSrcDir) {
#        win32 {
#            !contains(1, ^.:.*):1 = $$OUT_PWD/$$1
#        } else {
#            !contains(1, ^/.*):1 = $$OUT_PWD/$$1
#        }
#        out = $$cleanPath($$1)
#        out ~= s|^$$re_escape($$_PRO_FILE_PWD_/)||$$i_flag
#        return($$out)
#    }
#
#} # qt5

#-------------------------------------------------
# GLOBAL PROJECT VARIABLES
#-------------------------------------------------
NF_SOURCE_TREE = $$PWD
isEmpty(NF_BUILD_TREE) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    NF_BUILD_TREE = $$cleanPath($$OUT_PWD)
    NF_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
}
NF_APP_PATH = $$NF_BUILD_TREE/bin
macx {
    NF_APP_TARGET   = "DatFind"
    NF_LIBRARY_PATH = $$NF_APP_PATH/$${NF_APP_TARGET}.app/Contents/PlugIns
    NF_PLUGIN_PATH  = $$NF_APP_PATH/$${NF_APP_TARGET}.app/Contents/PlugIns
    NF_DATA_PATH    = $$NF_APP_PATH/$${NF_APP_TARGET}.app/Contents/Resources
    NF_DOC_PATH     = $$NF_APP_PATH/$${NF_APP_TARGET}.app/Contents/Resources/doc
    NF_BIN_PATH     = $$NF_APP_PATH/$${NF_APP_TARGET}.app/Contents/MacOS
    copydata = 1
    isEmpty(TIGER_COMPAT_MODE):TIGER_COMPAT_MODE=$$(QTC_TIGER_COMPAT)
    !isEqual(QT_MAJOR_VERSION, 5) {
        # Qt5 doesn't support 10.5, and will set the minimum version correctly to 10.6 or 10.7.
        isEmpty(TIGER_COMPAT_MODE) {
            QMAKE_CXXFLAGS *= -mmacosx-version-min=10.5
            QMAKE_LFLAGS *= -mmacosx-version-min=10.5
        }
    }
} else {
    contains(TEMPLATE, vc.*):vcproj = 1
    NF_APP_TARGET   = datfind
    NF_LIBRARY_PATH = $$NF_BUILD_TREE/lib/
    NF_PLUGIN_PATH  = $$NF_BUILD_TREE/lib/plugins
    NF_DATA_PATH    = $$NF_BUILD_TREE/share
    NF_DOC_PATH     = $$NF_BUILD_TREE/share/doc
    NF_BIN_PATH     = $$NF_APP_PATH
    !isEqual(NF_SOURCE_TREE, $$NF_BUILD_TREE):copydata = 1
}


#-------------------------------------------------
# INCLUDES
#-------------------------------------------------
#INCLUDEPATH += $$NF_SOURCE_TREE/include

#CONFIG += depend_includepath

#LIBS += -L$$NF_LIBRARY_PATH

#-------------------------------------------------
# OUTPUT
#-------------------------------------------------
#OBJECTS_DIR = $$NF_BUILD_TREE/.obj
#UI_DIR  = $$NF_BUILD_TREE/.uic
#MOC_DIR = $$NF_BUILD_TREE/.moc
#RCC_DIR = $$NF_BUILD_TREE/.rcc


#-------------------------------------------------
# DISABLE ASCII
#-------------------------------------------------
# DEFINES += QT_CREATOR QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII
# !macx:DEFINES += QT_USE_FAST_OPERATOR_PLUS QT_USE_FAST_CONCATENATION





