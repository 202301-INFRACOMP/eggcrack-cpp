set(FACTORY "factory/IdSequencer.hpp" "factory/Product.hpp" "factory/ProductFactory.hpp" "factory/ProductFactoryBuilder.hpp")
set(WORKER "factory/worker/BlueWorker.hpp" "factory/worker/OrangeWorker.hpp" "factory/worker/RedWorker.hpp")
set(MAILBOX "storage/mailbox/Mailbox.hpp" "storage/mailbox/FiniteMailbox.hpp" "storage/mailbox/InfiniteMailbox.hpp")

add_library(eggcrack INTERFACE "${SYNCHRONIZATION}" "${FACTORY}" "${WORKER}" "${STORAGE}")
target_include_directories(eggcrack INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_features(eggcrack INTERFACE cxx_std_20)
set_target_properties(eggcrack PROPERTIES CXX_EXTENSIONS OFF)
