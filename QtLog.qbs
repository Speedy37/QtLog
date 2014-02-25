import qbs 1.0

Product {
    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ["core"] }
    name: "qtlog"
    type: project.staticBuild ? "staticlibrary" : "dynamiclibrary"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    destinationDirectory: qbs.targetOS.contains("windows") ? "bin" : "lib"

    cpp.includePaths: [
        ".",
        ".."     // for the plugin headers
    ]
    cpp.defines: [
        product.type === "staticlibrary" ? "QTLOG_STATIC" : "QTLOG_LIBRARY"
    ]

    Properties {
        condition: qbs.toolchain.contains("msvc")
        cpp.cxxFlags: ["/W3", "/w34100", "/w34189"]
    }
    Properties {
        condition: qbs.toolchain.contains("gcc") && !qbs.targetOS.contains("windows")
        cpp.cxxFlags: ["-Werror"]
    }

    cpp.installNamePrefix: "@rpath/"

    files: ["qtlog.h", "qtlog.cpp", "qtlog_global.h"]

    Group {
        fileTagsFilter: product.type.concat("dynamiclibrary_symlink")
        qbs.install: true
        qbs.installDir: project.libInstallDir || ""
    }

    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["core"] }
        cpp.includePaths: path
        cpp.defines: product.type === "staticlibrary" ? ["QTLOG_STATIC"] : []
    }
}
