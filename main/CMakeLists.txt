idf_component_register(SRCS "main.cpp"
                            "network/network.cpp"
                            "webserver/webserver.cpp"
                    INCLUDE_DIRS "." "network" "webserver"
                    EMBED_TXTFILES "dashboard/index.html"
                                   "dashboard/style.css"
                                   "dashboard/script.js")
