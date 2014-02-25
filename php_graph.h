
#ifndef PHP_GRAPH_H
#define PHP_GRAPH_H 1

#define PHP_GRAPH_VERSION "1.0"
#define PHP_GRAPH_EXTNAME "graph"

PHP_FUNCTION(random_graph);

extern zend_module_entry graph_module_entry;
#define phpext_graph_ptr &graph_module_entry

#endif
