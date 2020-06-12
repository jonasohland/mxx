/** @file */
#pragma once

namespace mxx {

    template <typename user_class>
    struct object_wrapper {

        union object_header_type {
            c74::max::t_object maxobj_header;
            c74::max::t_pxobject mspobj_header;
        };

        operator c74::max::t_object *() const noexcept
        {
            return &object_header.maxobj_header;
        }

        operator c74::max::t_pxobject *() const noexcept
        {
            return &object_header.mspobj_header;
        }

        bool mxx_internal_init(t_atom_span args, long attr_offset)
        {
            // clang-format off
            
            assert(args.size() >= attr_offset);

            if constexpr(type_traits::has_construct_function<user_class>()){
                try {
                    object.construct(atom::vector(
                        args.begin(), args.begin() + attr_offset));
                }
                catch (std::runtime_error ex) {
                    c74::max::object_error(nullptr, ex.what());
                    return false;
                }
            }

            // clang-format on

            object.mxx_internal_finalize();

            c74::max::attr_args_process(
                &object_header, static_cast<short>(args.size()), args.begin());

            return true;
        }

        void mxx_internal_finish()
        {
            object_header.mspobj_header.z_misc |= object.mspflags();
        }

        object_header_type object_header;
        user_class object;
    };

    template <typename user_class>
    c74::max::t_class* wrapper_class_new(c74::max::t_class* class_ptr,
                                         const char* external_name,
                                         c74::max::method new_instance,
                                         c74::max::method free_instance,
                                         c74::max::method named_meth_wrapper,
                                         c74::max::method inth,
                                         c74::max::method bangh,
                                         c74::max::method floath,
                                         c74::max::method listh,
                                         c74::max::method anyh,
                                         c74::max::method assisth,
                                         c74::max::method inletinfoh,
                                         c74::max::method inputchangedh,
                                         c74::max::method multichanneloutputsh,
                                         c74::max::method dsp64h,
                                         c74::max::method dsp64h_user)
    {
        class_ptr = c74::max::class_new(external_name,
                                        new_instance,
                                        free_instance,
                                        sizeof(object_wrapper<user_class>),
                                        0L,
                                        c74::max::A_GIMME,
                                        0);

        if constexpr (type_traits::is_dsp_class<user_class>()) {
            c74::max::class_dspinit(class_ptr);
        }

        if constexpr (type_traits::has_bang_handler<user_class>()) {
            c74::max::class_addmethod(class_ptr, bangh, "bang");
        }


        if constexpr (type_traits::has_int_handler<user_class>()) {
            c74::max::class_addmethod(
                class_ptr, inth, "int", c74::max::A_LONG, 0);
        }

        if constexpr (type_traits::has_float_handler<user_class>()) {
            c74::max::class_addmethod(
                class_ptr, floath, "float", c74::max::A_FLOAT, 0);
        }

        if constexpr (type_traits::has_list_handler<user_class>()
                      || type_traits::has_raw_list_handler<user_class>()) {
            c74::max::class_addmethod(
                class_ptr, listh, "list", c74::max::A_GIMME, 0);
        }

        if constexpr (type_traits::has_any_msg_handler<user_class>()) {
            c74::max::class_addmethod(
                class_ptr, anyh, "anything", c74::max::A_GIMME, 0);
        }

        if constexpr (type_traits::has_dsp_handler<user_class>()) {
            c74::max::class_addmethod(
                class_ptr, dsp64h, "dsp64", c74::max::A_CANT, 0);
        }
        else if constexpr (type_traits::has_setup_dsp_function<user_class>())
            c74::max::class_addmethod(
                class_ptr, dsp64h_user, "dsp64", c74::max::A_CANT, 0);


        c74::max::class_addmethod(
            class_ptr, assisth, "assist", c74::max::A_CANT, 0);
        c74::max::class_addmethod(
            class_ptr, inletinfoh, "inletinfo", c74::max::A_CANT, 0);

        c74::max::class_addmethod(class_ptr,
                                  multichanneloutputsh,
                                  "multichanneloutputs",
                                  c74::max::A_CANT,
                                  0);

        c74::max::class_addmethod(
            class_ptr, inputchangedh, "inputchanged", c74::max::A_CANT, 0);

        user_class dummy;

        for (const auto& msg : dummy.messages()) {

            auto [name, handler] = msg;

            c74::max::class_addmethod(class_ptr,
                                      named_meth_wrapper,
                                      name.c_str(),
                                      handler->type(),
                                      0);
        }

        return class_ptr;
    }

    template <typename user_class>
    void* wrapper_object_new(c74::max::t_class* class_ptr,
                             c74::max::t_symbol* name,
                             long ac,
                             c74::max::t_atom* av)
    {
        static_assert(std::is_constructible<user_class>(),
                      "External class must be constructible without arguments");

        // create new maxobject instance
        auto* obj     = c74::max::object_alloc(class_ptr);
        auto* wrapper = reinterpret_cast<object_wrapper<user_class>*>(obj);

        // place user class into it
        try {
            new (&wrapper->object) user_class();
        }
        catch (std::exception ex) {
            return nullptr;
        };

        // prepare wrapper (by assigning a pointer to the object header)
        wrapper->object.mxx_internal_prepare(
            static_cast<c74::max::t_object*>(obj));

        // this symbol will tell us if we are dummy-instanciated or not
        static symbol tsym("__mxx_test_symbol__");

        // initialize user class (run constructor and create inlets/outlets and
        // stuff)
        if (static_cast<c74::max::t_symbol*>(tsym)) {
            if (!wrapper->mxx_internal_init(
                    detail::to_span(av, ac),
                    c74::max::attr_args_offset(static_cast<short>(ac), av)))
                return nullptr;
        }

        // dspsetup if dsp class
        if constexpr (type_traits::is_dsp_class<user_class>()) {
            c74::max::dsp_setup(static_cast<c74::max::t_pxobject*>(obj),
                                wrapper->object.streams());
        }

        wrapper->mxx_internal_finish();

        return obj;
    }

    template <typename user_class>
    void wrapper_object_free(c74::max::t_object* instance)
    {
        auto* wrapper = reinterpret_cast<object_wrapper<user_class>*>(instance);

        if constexpr (type_traits::is_dsp_class<user_class>())
            c74::max::dsp_free(
                reinterpret_cast<c74::max::t_pxobject*>(wrapper));

        wrapper->object.~user_class();
    }

    template <typename user_class>
    MXX_ALWAYS_INLINE void wrapper_msg_call(c74::max::t_object* o,
                                            c74::max::t_symbol* s,
                                            long ac,
                                            c74::max::t_atom* av)
    {
        auto args = t_atom_span(av, ac);

        reinterpret_cast<object_wrapper<user_class>*>(o)->object.call(
            s->s_name,
            std::make_shared<atom::vector>(args.begin(), args.end()));
    }

}    // namespace mxx
