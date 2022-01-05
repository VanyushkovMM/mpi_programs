#file(GLOB hdrs "*.h*") #Добавляем в переменную hdrs все файлы с расширением .h
file(GLOB srcs "*.cpp") #Добавляем в переменную srcs все файлы с расширением .h

add_executable(${main} ${srcs} ${hdrs})
target_link_libraries(${main} ${max_vector_value} ${trapez_method} ${vector_alternations} ${dijkstra})

if (UNIX)
    target_link_libraries(${main} pthread)
endif (UNIX)