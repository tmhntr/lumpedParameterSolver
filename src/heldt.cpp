#include "component/RCR.hpp"
#include "component/component_wrapper.hpp"
#include "component/heart.hpp"

component* heldt()
{
    RCR* body[10];
    body[0] = new RCR("Arteries", { "Q_up", "Q_kid1", "Q_spl1", "Q_ll1" }, { "P_lv", "P_a" }, { "Q_lo" }, { "P_a" }, { 0.006, 2.0 });
    body[0]->setHasValve(true);
    body[1] = new RCR("Upper Body", { "P_a", "P_up", "Q_sup" }, { "Q_up" }, { "P_up" }, { 3.9, 8.0 });
    body[2] = new RCR("Kidneys", { "P_a", "P_kid", "Q_kid2" }, { "Q_kid1" }, { "P_kid" }, { 4.1, 15.0 });
    body[3] = new RCR("Splanchnic", { "Q_spl2" }, { "P_a", "P_spl" }, { "Q_spl1" }, { "P_spl" }, { 3.0, 55.0 });
    body[4] = new RCR("Lower Legs", { "Q_ll2" }, { "P_a", "P_ll" }, { "Q_ll1" }, { "P_ll" }, { 3.6, 19.0 });
    body[5] = new RCR("Sup. Vena Cava", { "Q_ri1" }, { "P_up", "P_sup" }, { "Q_sup" }, { "P_sup" }, { 0.23, 15.0 });
    body[5]->setHasValve(true);
    body[6] = new RCR("Abd. Vena Cava", { "Q_inf" }, { "P_kid", "P_spl", "P_ll", "P_abd" }, { "Q_kid2", "Q_spl2", "Q_ll2" }, { "P_abd" }, { 0.3, 0.18, 0.3, 25.0 });
    body[6]->setHasValve(true);
    body[7] = new RCR("Inf. Vena Cava", { "Q_ri2" }, { "P_abd", "P_inf" }, { "Q_inf" }, { "P_inf" }, { 0.015, 2.0 });
    body[7]->setHasValve(true);
    body[8] = new RCR("Pulm. Arteries", { "Q_pv" }, { "P_rv", "P_pa" }, { "Q_pa" }, { "P_pa" }, { 0.003, 4.3 });
    body[8]->setHasValve(true);
    body[9] = new RCR("Pulm. Veins", { "Q_li" }, { "P_pa", "P_pv" }, { "Q_pv" }, { "P_pv" }, { 0.08, 8.4 });

    heart* hrt = new heart({ "Q_pv", "Q_lo" }, { "C_ra", "C_rv", "C_la", "C_lv", "P_ra", "P_rv", "P_la", "P_lv", "P_sup", "P_inf", "P_pv" }, { "C_ra", "C_rv", "C_la", "C_lv", "Q_ri1", "Q_ri2", "Q_rav", "Q_li", "Q_lav" }, { "C_ra", "C_rv", "C_la", "C_lv", "P_ra", "P_rv", "P_la", "P_lv" }, { 0.8, 2.5, 0.13, 1.3, 0.07, 0.61, 0.5, 0.74, 0.3, 0.06, 0.015, 0.005, 0.01, 0.01 });

    component_wrapper* mdl = new wrapper(std::vector<model*> { hrt });
    for (int i = 0; i < 10; i++)
        mdl->addModel(body[i]);

    mdl->init(mdl);
    return mdl;
}
