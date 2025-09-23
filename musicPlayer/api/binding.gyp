{
  "targets": [
    {
      "target_name": "music_api",
      "sources": [
        "node_music_api.cpp",
        "musicAPI.cpp",
        "../core/sqlSetup.cpp"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "../core"
      ],
      "libraries": [
        "-lsqlite3"
      ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "conditions": [
        ["OS=='mac'", {
          "cflags_cc": [ "-std=c++20", "-stdlib=libc++" ],
          "xcode_settings": {
            "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
            "CLANG_CXX_LIBRARY": "libc++",
            "MACOSX_DEPLOYMENT_TARGET": "10.7",
            "CLANG_CXX_LANGUAGE_STANDARD": "c++20",
            "OTHER_CPLUSPLUSFLAGS": [ "-std=c++20" ]
         }
        }],
        ["OS=='linux'", {
          "cflags_cc": [ "-std=c++20" ]
        }]
      ],
      "msvs_settings": {
        "VCCLCompilerTool": { "ExceptionHandling": 1 }
      }
    }
  ]
}
