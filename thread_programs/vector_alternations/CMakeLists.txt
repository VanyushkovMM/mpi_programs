file(GLOB hdrs "*.h*") #Добавляем в переменную hdrs все файлы с расширением .h
file(GLOB srcs "*.cpp")#Добавляем в переменную srcs все файлы с расширением .h

add_library(${vector_alternations} STATIC ${srcs} ${hdrs}) 

if (UNIX)
    target_link_libraries(${vector_alternations} pthread)
endif (UNIX)