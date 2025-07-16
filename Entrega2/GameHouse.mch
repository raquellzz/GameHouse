MACHINE GameHouse

INCLUDES GameHouse_ctx

VARIABLES
    tiposDispositivos,
    dispositivos,
    tipoDispositivo,
    statusDispositivo,
    jogos,
    tipoJogo,
    jogosDispositivo, 
    clientes,
    statusCliente,
    creditosCliente,
    sessao,
    jogoAtual,
    filaDispositivo, 
    jogoSolicitado
INVARIANT
    tiposDispositivos <: TIPOS_DISPOSITIVO 
    & dispositivos <: DISPOSITIVOS 
    & dispositivos : FIN(dispositivos)
    & card(dispositivos) <= MAX_DISPOSITIVOS 
    & tipoDispositivo : dispositivos --> tiposDispositivos
    & statusDispositivo : dispositivos --> STATUS_DISPOSITIVO 
    
    & jogos <: JOGOS 
    & tipoJogo : jogos --> tiposDispositivos 
    & jogosDispositivo : dispositivos <-> jogos 
    & !(dd,jj).(dd |-> jj : jogosDispositivo => (dd : dispositivos & jj : jogos & tipoDispositivo(dd) = tipoJogo(jj))) 
    
    & clientes <: CLIENTES 
    & statusCliente : clientes --> STATUS_CLIENTE
    & creditosCliente : clientes --> NAT
    & creditosCliente : FIN(creditosCliente)
    
    & sessao : clientes >+> dispositivos 
    & jogoAtual : sessao --> jogos 
    & !(cc,dd,jj).(cc |-> dd : sessao & cc |-> dd |-> jj : jogoAtual => dd |-> jj : jogosDispositivo) 
    & !cc.(cc : dom(sessao) => statusCliente(cc) = ativo) 
    & !dd.(dd : ran(sessao) => statusDispositivo(dd) = em_uso) 
    & filaDispositivo : dispositivos --> seq(clientes) 
    & jogoSolicitado : clientes +-> jogos 
    & !(dd1,dd2,ii,jj).(dd1 : dispositivos 
        & dd2 : dispositivos 
        & dd1 /= dd2 
        & ii : dom(filaDispositivo(dd1)) 
        & jj : dom(filaDispositivo(dd2)) => filaDispositivo(dd1)(ii) /= filaDispositivo(dd2)(jj)) 
    & !(dd,ii).(dd : dispositivos & ii : dom(filaDispositivo(dd)) => (statusCliente(filaDispositivo(dd)(ii)) = em_espera & filaDispositivo(dd)(ii) /: dom(sessao))) 
    & !(cc).(cc : clientes & statusCliente(cc) = em_espera => cc : dom(jogoSolicitado))

INITIALISATION 
    tiposDispositivos := {} ||
    dispositivos := {} ||
    tipoDispositivo := {} ||
    statusDispositivo := {} ||
    jogos := {} ||
    tipoJogo := {} ||
    jogosDispositivo := {} ||
    clientes := {} ||
    statusCliente := {} ||
    creditosCliente := {} ||
    sessao := {} ||
    jogoAtual := {} ||
    filaDispositivo := {} ||
    jogoSolicitado := {}

OPERATIONS

    add_tipo_dispositivo(td) =
        PRE
            td : TIPOS_DISPOSITIVO &
            td /: tiposDispositivos
        THEN
            tiposDispositivos := tiposDispositivos \/ {td}
        END;

    remove_tipo_dispositivo(td) =
        PRE
            td : TIPOS_DISPOSITIVO &
            td /: ran(tipoDispositivo) &
            td /: ran(tipoJogo)
        THEN
            IF  
            td : tiposDispositivos
            THEN
            tiposDispositivos := tiposDispositivos - {td}
            END
        END;

    add_dispositivo(dd, td) =
        PRE
            dd : DISPOSITIVOS &
            dd /: dispositivos &
            td : TIPOS_DISPOSITIVO &
            card(dispositivos) < MAX_DISPOSITIVOS
        THEN 
            IF  
            td : tiposDispositivos
            THEN
                
            dispositivos := dispositivos \/ {dd} ||
            tipoDispositivo := tipoDispositivo \/ {dd |-> td} ||
            statusDispositivo := statusDispositivo \/ {dd |-> disponivel} ||
            filaDispositivo := filaDispositivo \/ {dd |-> []}
            END
        END;
        
    remove_dispositivo(dd) =
        PRE
            dd : DISPOSITIVOS &
            statusDispositivo(dd) /= em_uso &
            filaDispositivo(dd) = []
        THEN
            IF dd : dispositivos
            THEN
                dispositivos := dispositivos - {dd} ||
                tipoDispositivo := {dd} <<| tipoDispositivo ||
                statusDispositivo := {dd} <<| statusDispositivo ||
                jogosDispositivo := {dd} <<| jogosDispositivo ||
                filaDispositivo := {dd} <<| filaDispositivo
            END
        END;

    add_jogo(jj, td) =
        PRE
            jj : JOGOS &
            jj /: jogos &
            td : TIPOS_DISPOSITIVO 
        THEN 
            IF  
            td : tiposDispositivos
            THEN
            jogos := jogos \/ {jj} ||
            tipoJogo := tipoJogo \/ {jj |-> td}
            END
        END;
        
    remove_jogo(jj) =
        PRE
            jj : JOGOS & 
            jj /: ran(jogoAtual) &
            jj /: ran(jogoSolicitado)
        THEN
            IF  jj : jogos
            THEN
            jogos := jogos - {jj} ||
            tipoJogo := {jj} <<| tipoJogo ||
            jogosDispositivo := jogosDispositivo |>> {jj}
            END
        END;
        
    instalar_jogo(dd, jj) =
        PRE
            dd : DISPOSITIVOS &
            jj : JOGOS &
            tipoDispositivo(dd) = tipoJogo(jj) &
            dd |-> jj /: jogosDispositivo
        THEN
            IF  jj : jogos &  dd : dispositivos
            THEN
            jogosDispositivo := jogosDispositivo \/ {dd |-> jj}
            END
        END;
        
    desinstalar_jogo(dd,jj) =
        PRE
            dd : DISPOSITIVOS &
            jj : JOGOS &
            jj /: ran(jogoAtual) &
            dd /: ran(sessao) &
            tipoDispositivo(dd) = tipoJogo(jj) &
            dd |-> jj : jogosDispositivo &
            jj /: ran(jogoSolicitado)
        THEN
            IF  jj : jogos &  dd : dispositivos
            THEN
            jogosDispositivo := jogosDispositivo - {dd |-> jj}
            END
        END;
        
    set_status_dispositivo(dd, ss) =
        PRE
            dd : DISPOSITIVOS &
            ss : STATUS_DISPOSITIVO &
            (ss = em_uso => dd : ran(sessao)) &
            (ss /= em_uso => dd /: ran(sessao))
        THEN
            IF dd : dispositivos
            THEN
            statusDispositivo(dd) := ss
            END
        END;
        
    add_cliente(cc) =
        PRE
            cc : CLIENTES & cc /: clientes
        THEN
            clientes := clientes \/ {cc} ||
            statusCliente := statusCliente \/ {cc |-> inativo} ||
            creditosCliente := creditosCliente \/ {cc |-> 0}
        END;
    
    banir_cliente(cc) =
        PRE
            cc : CLIENTES &
            cc /: dom(sessao) &
            !dd.(dd : dispositivos => cc /: ran(filaDispositivo(dd)))
        THEN
            IF cc : clientes
            THEN
            clientes := clientes - {cc} ||
            statusCliente := {cc} <<| statusCliente ||
            creditosCliente := {cc} <<| creditosCliente ||
            jogoSolicitado := {cc} <<| jogoSolicitado
            END
        END;
        
    comprar_creditos(cc, qtd) =
        PRE
            cc : CLIENTES &
            qtd : 1..3 
        THEN
            IF cc : clientes &
            creditosCliente(cc) + qtd <= MAX_CREDITOS
            THEN
            creditosCliente(cc) := creditosCliente(cc) + qtd
            END
        END;

    pp <-- posicao_fila(cc, dd) =
        PRE
            cc : CLIENTES &
            dd : DISPOSITIVOS &
            cc : ran(filaDispositivo(dd))
        THEN
            IF  cc : clientes &  dd : dispositivos
            THEN
                ANY pos WHERE
                    pos : 1..size(filaDispositivo(dd)) &
                    filaDispositivo(dd)(pos) = cc
                THEN
                    pp := pos
                END
            END
        END;

    entrar_fila_dispositivo(cc, dd, jj) =
        PRE
            cc : CLIENTES &
            dd : DISPOSITIVOS &
            jj : JOGOS &
            dd |-> jj : jogosDispositivo &
            statusCliente(cc) = inativo &
            cc /: dom(sessao) &
            !dd.(dd : dispositivos => cc /: ran(filaDispositivo(dd))) &
            creditosCliente(cc) > 0
        THEN
            IF  cc : clientes &  dd : dispositivos & jj : jogos
            THEN
            filaDispositivo(dd) := filaDispositivo(dd) <- cc ||
            jogoSolicitado := jogoSolicitado \/ {cc |-> jj} ||
            statusCliente(cc) := em_espera
            END
        END;
        
    sair_fila_dispositivo(cc, dd) =
        PRE
            cc : CLIENTES &
            dd : DISPOSITIVOS &
            cc : ran(filaDispositivo(dd)) &
            statusCliente(cc) = em_espera
        THEN
            IF  cc : clientes &  dd : dispositivos
            THEN
                ANY nova_fila WHERE
                    nova_fila : seq(clientes) &
                    ran(nova_fila) <: clientes &
                    ran(nova_fila) = ran(filaDispositivo(dd)) - {cc} &
                    size(nova_fila) = size(filaDispositivo(dd)) - 1
                THEN
                    filaDispositivo(dd) := nova_fila ||
                    statusCliente(cc) := inativo ||
                    jogoSolicitado := {cc} <<| jogoSolicitado
                END
            END
        END;
       
    iniciar_sessao_fila(dd) =
        PRE
            dd : DISPOSITIVOS &
            statusDispositivo(dd) = disponivel &
            filaDispositivo(dd) /= []
        THEN
            IF  dd : dispositivos
            THEN
                ANY cc WHERE
                    cc = first(filaDispositivo(dd))
                THEN
                    ANY jj, resto_fila WHERE
                        jj = jogoSolicitado(cc) &
                        resto_fila = tail(filaDispositivo(dd))
                    THEN
                        statusDispositivo(dd) := em_uso ||
                        statusCliente(cc) := ativo ||
                        creditosCliente(cc) := creditosCliente(cc) - 1 ||
                        sessao := sessao \/ {cc |-> dd} ||
                        jogoAtual := jogoAtual \/ {cc |-> dd |-> jj} ||
                        filaDispositivo(dd) := resto_fila ||
                        jogoSolicitado := {cc} <<| jogoSolicitado
                    END
                END
            END
        END;
        
    encerrar_sessao(cc) =
        PRE
            cc : CLIENTES
        THEN
            IF cc : dom(sessao)
            THEN
                ANY dd WHERE
                    dd = sessao(cc)
                THEN
                    statusDispositivo(dd) := disponivel ||
                    statusCliente(cc) := inativo ||
                    sessao := {cc} <<| sessao ||
                    jogoAtual := {cc |-> dd} <<| jogoAtual
                END
            END
        END;
        
/*-------------- Operações para garantir precondições das operações existentes --------------*/  

    ok <-- pre_add_tipo_dispositivo(td) =
        PRE
            td : TIPOS_DISPOSITIVO
        THEN
            IF td /: tiposDispositivos
            THEN ok := TRUE
            ELSE ok := FALSE
            END
        END;
    
    ok <-- pre_remove_tipo_dispositivo(td) =
        PRE
            td : TIPOS_DISPOSITIVO
        THEN
            IF td : tiposDispositivos & td /: ran(tipoDispositivo) & td /: ran(tipoJogo)
            THEN ok := TRUE
            ELSE ok := FALSE
            END
        END;
    
    ok <-- pre_add_dispositivo(dd, td) =
        PRE
            dd : DISPOSITIVOS &
            td : TIPOS_DISPOSITIVO
        THEN
            IF dd /: dispositivos & td : tiposDispositivos & card(dispositivos) < MAX_DISPOSITIVOS
            THEN ok := TRUE
            ELSE ok := FALSE
            END
        END;

    ok <-- pre_remove_dispositivo(dd) =
        PRE
            dd : DISPOSITIVOS
        THEN
            IF dd : dispositivos & statusDispositivo(dd) /= em_uso & filaDispositivo(dd) = []
            THEN ok := TRUE
            ELSE ok := FALSE
            END
        END;
        
    ok <-- pre_add_jogo(jj, td) =
        PRE
            jj : JOGOS &
            td : TIPOS_DISPOSITIVO
        THEN
            IF jj /: jogos & td : tiposDispositivos
            THEN ok := TRUE
            ELSE ok := FALSE
            END
        END;
    
    ok <-- pre_remover_jogo(jj) =
        PRE
            jj : JOGOS
        THEN
            IF jj : jogos & jj /: ran(jogoAtual) & jj /: ran(jogoSolicitado)
            THEN ok := TRUE
            ELSE ok := FALSE
            END
        END;
    
    ok <-- pre_instalar_jogo(dd, jj) =
        PRE
            dd : DISPOSITIVOS &
            jj : JOGOS
        THEN
            IF dd : dispositivos & jj : jogos & tipoDispositivo(dd) = tipoJogo(jj) & dd |-> jj /: jogosDispositivo
            THEN ok := TRUE
            ELSE ok := FALSE
            END
        END;
    
    ok <-- pre_desinstalar_jogo(dd, jj) =
        PRE
            dd : DISPOSITIVOS &
            jj : JOGOS
        THEN
            IF dd : dispositivos & jj : jogos & jj /: ran(jogoAtual) & dd /: ran(sessao) & 
               tipoDispositivo(dd) = tipoJogo(jj) & dd |-> jj : jogosDispositivo & jj /: ran(jogoSolicitado)
            THEN ok := TRUE
            ELSE ok := FALSE
            END
        END;
    
    ok <-- pre_banir_cliente(cc) =
        PRE
            cc : CLIENTES
        THEN
            IF cc : clientes & cc /: dom(sessao) & 
               (!dd.(dd : dispositivos => cc /: ran(filaDispositivo(dd))))
            THEN ok := TRUE
            ELSE ok := FALSE
            END
        END;
    
    ok <-- pre_comprar_creditos(cc, qtd) =
        PRE
            cc : CLIENTES &
            qtd : NAT
        THEN
            IF cc : clientes & qtd : 1..MAX_CREDITOS & creditosCliente(cc) + qtd <= MAX_CREDITOS
            THEN ok := TRUE
            ELSE ok := FALSE
            END
        END;
    
    ok <-- pre_set_status_dispositivo(dd, ss) =
        PRE
            dd : DISPOSITIVOS &
            ss : STATUS_DISPOSITIVO
        THEN
            IF (ss = em_uso => dd : ran(sessao)) &
                dd : dispositivos &
                (ss /= em_uso => dd /: ran(sessao))
            THEN ok := TRUE
            ELSE ok := FALSE
            END
        END;
        
    ok <-- pre_add_cliente(cc) =
        PRE
            cc : CLIENTES
        THEN
            IF cc /: clientes
            THEN ok := TRUE
            ELSE ok := FALSE
            END
        END;
        
    ok <-- pre_entrar_fila_dispositivo(cc, dd, jj) = 
        PRE cc : CLIENTES &
            dd : DISPOSITIVOS &
            jj : JOGOS
        THEN    
            IF cc : clientes &
                dd : dispositivos &
                jj : jogos  &
                cc /: dom(sessao) &
                dd |-> jj : jogosDispositivo &
                statusCliente(cc) = inativo &
                !dd.(dd : dispositivos => cc /: ran(filaDispositivo(dd))) &
                creditosCliente(cc) > 0 
            THEN ok := TRUE
            ELSE ok := FALSE
            END
        END;
    
    ok <-- pre_sair_fila_dispositivo(cc, dd) = 
        PRE cc : CLIENTES &
            dd : DISPOSITIVOS
        THEN    
            IF  cc : clientes &
                dd : dispositivos &
                cc : ran(filaDispositivo(dd)) &
                statusCliente(cc) = em_espera
            THEN ok := TRUE
            ELSE ok := FALSE
            END
        END;
    
    ok <-- pre_iniciar_sessao(dd) = 
        PRE dd : DISPOSITIVOS
        THEN    
            IF dd : dispositivos &
                statusDispositivo(dd) = disponivel &
                filaDispositivo(dd) /= []
            THEN ok := TRUE
            ELSE ok := FALSE
            END
        END;
        
    ok <-- pre_encerrar_sessao(cc) = 
        PRE
            cc : CLIENTES
        THEN
            IF cc : dom(sessao)
            THEN
            ok := TRUE
            END
        END;

    init_disp_disponiveis =
        skip;

    more <-- has_more_disp_disponiveis =
        BEGIN 
            more :: BOOL 
        END;

    dd <-- get_next_disp_disponivel =
        BEGIN 
            dd :: DISPOSITIVOS
        END;


    /* ----- Iterador para Jogos por Tipo ----- */
    init_jogos_por_tipo =
        skip;

    more <-- has_more_jogos_por_tipo(td) =
        PRE 
            td : TIPOS_DISPOSITIVO
        THEN 
            more :: BOOL 
        END;

    jj <-- get_next_jogo_por_tipo(td) =
        PRE 
            td : TIPOS_DISPOSITIVO
        THEN 
            jj :: JOGOS 
        END;


    /* ----- Iterador para Jogos por Dispositivo ----- */
    init_jogos_dispositivo =
        skip;

    more <-- has_more_jogos_dispositivo(dd) =
        PRE 
            dd : DISPOSITIVOS 
        THEN 
            more :: BOOL 
        END;

    jj <-- get_next_jogo_dispositivo(dd) =
        PRE 
            dd : DISPOSITIVOS 
        THEN 
            jj :: JOGOS 
        END;


    /* ----- Iterador para Informações de Clientes ----- */
    init_clientes_info =
        skip;

    more <-- has_more_clientes_info =
        BEGIN 
            more :: BOOL 
        END;

    cc, ss, cr <-- get_next_cliente_info =
        BEGIN
            cc :: CLIENTES || 
            ss :: STATUS_CLIENTE ||
            cr :: NAT
        END;
        
    init_tipos_disponiveis =
        skip;

    more <-- has_more_tipos_disponiveis =
        BEGIN 
            more :: BOOL 
        END;

    td <-- get_next_tipo_dispositivo =
        BEGIN 
            td :: TIPOS_DISPOSITIVO 
        END;
        
    init_devices_info =
        skip;

    more <-- has_more_devices_info =
        BEGIN 
            more :: BOOL 
        END;

    dd, td, ss <-- get_next_device_info =
        BEGIN
            dd :: DISPOSITIVOS ||
            td :: TIPOS_DISPOSITIVO ||
            ss :: STATUS_DISPOSITIVO
        END
         
END