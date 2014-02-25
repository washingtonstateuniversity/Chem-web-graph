
PHP_ARG_ENABLE(graph, whether to enable Graph support, [ --enable-graph Enable Graph support])

if test "$PHP_GRAPH" = "yes"; then
	AC_DEFINE(HAVE_GRAPH, 1, [Whether you have GRAPH])
	PHP_NEW_EXTENSION(graph, graph.c, $ext_shared)
fi

