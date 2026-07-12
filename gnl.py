import gdb

class VisualizaGNL(gdb.Command):
    """
    Comando para desenhar a t_gnl_list com ênfase na estrutura interna de cada t_gnl_node.
    Uso: gnl <nome_da_variavel_lista>
    Exemplo: gnl list
             gnl &contexts[0]
    """

    def __init__(self):
        super(VisualizaGNL, self).__init__("gnl", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        if not arg:
            print("\n[Erro] Passe a variavel da lista como argumento. Ex: gnl list")
            return

        try:
            gnl_list_val = gdb.parse_and_eval(arg)

            # Se for um ponteiro para a lista, faz a desreferência automática
            if gnl_list_val.type.code == gdb.TYPE_CODE_PTR:
                if int(gnl_list_val) == 0:
                    print("\n[ A lista passada eh NULL ]\n")
                    return
                gnl_list_val = gnl_list_val.dereference()

            head = gnl_list_val['head']
            tail = gnl_list_val['tail']

            print("\n" + "="*65)
            print("         VISUALIZADOR ANATÔMICO DE ESTRUTURAS (GNL)")
            print("="*65)
            
            # Exibe o estado da struct principal (t_gnl_list)
            print(f" ESTRUTURA DA LISTA (t_gnl_list):")
            print(f"  ├── head: {hex(int(head)) if int(head) != 0 else '0x0 (NULL)'}")
            print(f"  └── tail: {hex(int(tail)) if int(tail) != 0 else '0x0 (NULL)'}")
            print("-" * 65)

            if int(head) == 0:
                print("\n[ Lista Vazia - Head eh NULL ]\n")
                print("="*65 + "\n")
                return

            print(" MAPEAMENTO DOS NÓS (t_gnl_node):")
            
            current = head
            node_count = 0

            while int(current) != 0:
                node_count += 1
                node_addr = hex(int(current))
                content_ptr = current['content']
                content_addr = hex(int(content_ptr)) if int(content_ptr) != 0 else "0x0"
                next_node = current['next']
                next_addr = hex(int(next_node)) if int(next_node) != 0 else "0x0 (NULL)"
                
                # Captura e trata a string apontada por content
                if int(content_ptr) != 0:
                    try:
                        content_str = content_ptr.string()
                        # Correção aqui: tiramos o .replace de dentro do escopo da f-string
                        cleaned_str = content_str.replace("\n", "\\n")
                        display_str = f'"{cleaned_str}"'
                    except gdb.MemoryError:
                        display_str = "<ERRO DE MEMÓRIA / STRING INVÁLIDA>"
                else:
                    display_str = "NULL"

                # Identifica graficamente os papéis do nó atual
                flags = []
                if int(current) == int(head): flags.append("HEAD")
                if int(current) == int(tail): flags.append("TAIL")
                flag_str = f" <--- ({' & '.join(flags)})" if flags else ""

                # Desenha a anatomia isolada do nó (t_gnl_node)
                print(f"\n  [NÓ #{node_count}] Endereço: {node_addr}{flag_str}")
                print(f"   ├── char *content: {content_addr} -> {display_str}")
                print(f"   └── struct s_gnl_node *next: {next_addr}")

                # Se houver um próximo nó, desenha a seta de encadeamento
                if int(next_node) != 0:
                    print("                               │")
                    print("                               ▼")
                
                current = next_node

            print("\n" + "="*65)
            print(f" Total de nós (t_gnl_node) alocados na lista: {node_count}")
            print("="*65 + "\n")

        except gdb.error as e:
            print(f"\n[Erro do GDB]: {e}")
            print("Verifique se o argumento passado eh realmente do tipo t_gnl_list ou t_gnl_list*.\n")

VisualizaGNL()