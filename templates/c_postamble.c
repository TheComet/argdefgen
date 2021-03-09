#if !defined(ADG_FPRINTF)
#   define ADG_FPRINTF(...) fprintf(__VA_ARGS__)
#endif
#if !defined(ADG_FPRINTF_COLOR)
#   define ADG_FPRINTF_COLOR(print_func, fp, color, fmt, ...) \
        print_func(fp, color fmt, __VA_ARGS__)
#endif
#if !defined(ADG_COLOR_RESET)
#   define ADG_COLOR_RESET    "\u001b[0m"
#endif
#if !defined(ADG_COLOR_HEADING1)
#define ADG_COLOR_HEADING1 "\u001b[1;37m"
#endif
#if !defined(ADG_COLOR_HEADING2)
#define ADG_COLOR_HEADING2 "\u001b[1;34m"
#endif
#if !defined(ADG_COLOR_LONGOPT)
#define ADG_COLOR_LONGOPT  "\u001b[1;32m"
#endif
#if !defined(ADG_COLOR_SHORTOPT)
#define ADG_COLOR_SHORTOPT "\u001b[1;36m"
#endif
#if !defined(ADG_COLOR_ARG)
#define ADG_COLOR_ARG      "\u001b[1;36m"
#endif
#if !defined(ADG_COLOR_ERROR)
#   define ADG_COLOR_ERROR "\u001b[1;31m"
#endif

#if !defined(ADG_HELP_EXAMPLES)
#   define ADG_HELP_EXAMPLES
#endif

static const char* g_program_name;

struct handler
{
    char** args;
    int action_id;
};

struct queue_entry
{
    struct queue_entry* prev;
    struct handler* handler;
    int priority;
};

struct handler_queue
{
    struct queue_entry* front;
};

struct handler_list
{
    struct queue_entry* entries;
    int size;
};

/* ------------------------------------------------------------------------- */
static void list_init(struct handler_list* list)
{
    list->entries = NULL;
    list->size = 0;
}

/* ------------------------------------------------------------------------- */
static void queue_init(struct handler_queue* q)
{
    q->front = NULL;
}

/* ------------------------------------------------------------------------- */
static struct queue_entry* dequeue(struct handler_queue* q)
{
    struct queue_entry* entry;
    entry = q->front;
    if (entry)
        q->front = entry->prev;
    return entry;
}

/* ------------------------------------------------------------------------- */
static void enqueue(struct handler_queue* q, struct queue_entry* entry, int priority)
{
    struct queue_entry* infront = NULL;
    struct queue_entry* behind = q->front;

    entry->prev = NULL;
    entry->priority = priority;
    while (behind && behind->priority <= priority)
    {
        infront = behind;
        behind = behind->prev;
    }
    if (infront)
    {
        infront->prev = entry;
        entry->prev = behind;
    }
    else
    {
        q->front = entry;
        entry->prev = behind;
    }
}

/* ------------------------------------------------------------------------- */
static struct queue_entry* peek_front(struct handler_queue* q)
{
    return q->front;
}

/* ------------------------------------------------------------------------- */
static int parse_full_option(int argc, char** argv, struct handler_list* list)
{
    return 0;
}

/* ------------------------------------------------------------------------- */
static int parse_short_options(int argc, char** argv, struct handler_list* list)
{
    return 0;
}

/* ------------------------------------------------------------------------- */
static int parse_option(int argc, char** argv, struct handler_list* list)
{
    if (argv[0][0] == '-')
    {
        if (argv[0][1] == '-')
            return parse_full_option(argc, argv, list);
        else if (argv[0][1] != '\0')
            return parse_short_options(argc, argv, list);
    }

    ADG_FPRINTF(stderr, "Error: Unrecognized command line option `%s`\n", argv[0]);
    return 0;
}

/* ------------------------------------------------------------------------- */
int parse_command_line(int argc, char** argv)
{
    int i;
    struct handler_list list;
    struct handler_queue queue;
    list_init(&list);
    queue_init(&queue);

    g_program_name = argv[0];

    for (i = 1; i < argc; )
    {
        int processed = parse_option(argc - i, &argv[i], &list);
        if (processed == 0)
            return -1;
        i += processed;
    }

    return 0;
}
