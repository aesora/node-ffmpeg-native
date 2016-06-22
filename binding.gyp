{
  "targets": [
    {
      "target_name": "ffmpeg",
      "sources": [
        "src/ffmpeg.cpp",
        "src/avinput.cpp",
        "src/avinfo.cpp"
        ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "conditions": [
        ['OS=="mac"', {
          "cflags" : [
            "-g",
            "-D__STDC_CONSTANT_MACROS",
            "-D_FILE_OFFSET_BITS=64",
            "-D_LARGEFILE_SOURCE",
            "-O3",
            "-Wall"],
          "libraries" : [
              "-lavcodec",
              "-lavformat",
              "-lswscale",
              "-lavresample",
              "-lavutil"
          ]
        }],
        ['OS=="linux"', {
          "cflags" : [
            "-g",
            "-D__STDC_CONSTANT_MACROS",
            "-D_FILE_OFFSET_BITS=64",
            "-D_LARGEFILE_SOURCE",
            "-O3",
            "-Wall"],
          "libraries" : [
              "-lavcodec",
              "-lavformat",
              "-lswscale",
              "-lavresample",
              "-lavutil",
          ]
        }],
        ['OS=="win"', {
            "include_dirs": [
                "$(LIBAV_PATH)include"
                ],
            "libraries" : [
                  "-l$(LIBAV_PATH)avcodec",
                  "-l$(LIBAV_PATH)avformat",
                  "-l$(LIBAV_PATH)swscale",
                  "-l$(LIBAV_PATH)avresample",
                  "-l$(LIBAV_PATH)avutil"
                ]
        }]
      ],
    }
  ]
}
