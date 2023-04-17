use proc_macro::TokenStream;
use quote::quote;
use quote::ToTokens;
use syn;

#[proc_macro_derive(GenericInstruction)]
pub fn generic_instruction_derive(input: TokenStream) -> TokenStream {
    let ast = syn::parse(input).unwrap();

    impl_generic_instriction(&ast)
}

fn impl_generic_instriction(ast: &syn::DeriveInput) -> TokenStream {
    let name = &ast.ident;
    let uppercase_name = name.to_string().to_uppercase();

    let fields = match &ast.data {
        syn::Data::Struct(syn::DataStruct {
            fields: syn::Fields::Named(fields),
            ..
        }) => &fields.named,
        _ => panic!("expected a struct with named fields"),
    };

    let field_name = fields.iter().map(|field| &field.ident);
    let parse_fields = fields.iter().map(|field| {
        let fname = &field.ident;
        let fty = match &field.ty {
            syn::Type::Path(path) => path.clone().into_token_stream().to_string(),
            _ => panic!("Expecting only TypePath for attributes of generic instructions!"),
        };

        match fty.as_str() {
            "String" => {
                quote!(let #fname = chars.by_ref().take_while(|c| *c != ' ').collect::<String>();)
            }
            "Value" => quote!(let #fname = value::parse_value(&mut chars)?;),
            _ => panic!("Expecting only TypePath for attributes of generic instructions!"),
        }
    });

    let gen = quote! {
        impl GenericInstruction for #name {
            fn parse(str: &str) -> Result<Instruction, ParseError> {
                let mut chars = str.chars();
                #(#parse_fields)*
                Ok(Instruction::#name(#name {
                    #(
                        #field_name: #field_name,
                    )*
                }))
            }
        }

        impl #name {
            const NAME : &str = #uppercase_name;
        }

    };
    gen.into()
}
