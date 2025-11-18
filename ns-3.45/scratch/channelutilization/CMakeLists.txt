# yaml-cppのパスを明示的に指定
set(YAML_CPP_INCLUDE_DIR "/opt/homebrew/Cellar/yaml-cpp/0.8.0/include")
set(YAML_CPP_LIBRARY "/opt/homebrew/Cellar/yaml-cpp/0.8.0/lib/libyaml-cpp.0.8.0.dylib")

# ソースファイルを取得
file(GLOB scratch_sources CONFIGURE_DEPENDS *.cc)

# 実行可能ファイルをビルド
build_exec(
  EXECNAME channelutilization
  EXECNAME_PREFIX scratch_
  SOURCE_FILES "${scratch_sources}"
  LIBRARIES_TO_LINK "${ns3-libs}" "${ns3-contrib-libs}" "${YAML_CPP_LIBRARY}"
  EXECUTABLE_DIRECTORY_PATH ${CMAKE_CURRENT_BINARY_DIR}/
)

# インクルードディレクトリを追加
target_include_directories(scratch_channelutilization PRIVATE ${YAML_CPP_INCLUDE_DIR})