namespace ippl {

    namespace hdf5 {

        template <class ParticleContainer>
        ParticleStream<ParticleContainer>::ParticleStream(std::unique_ptr<ippl::Format> format)
            : Stream<ParticleContainer>(std::move(format)) {}

        template <class ParticleContainer>
        ParticleStream<ParticleContainer>& ParticleStream<ParticleContainer>::operator<<(
            const ParticleContainer& obj) {
            std::cout << "This is a HDF5 stream:" << std::endl;
            size_t nAttrib = obj.getAttributeNum();
            for (size_t i = 0; i < nAttrib; ++i) {
                auto attr = obj.getAttribute(i);

                bool present = false;
                try {
                    present = this->param_m.template get<bool>(attr->long_name());
                } catch (...) {
                    present = false;
                }

                if (present) {
                    hsize_t dims[1];
                    dims[0] = 50;
                    H5::DataSpace dataspace(1, dims);

                    std::cout << "Name:      " << attr->name() << std::endl;
                    H5::DataType attr_type = core::get_hdf5_type(attr->type());

                    // Create the dataset.
                    H5::DataSet dataset =
                        this->h5file_m.createDataSet(attr->name(), attr_type, dataspace);

                    //                     dataset = this->h5file_m.openDataSet(attr->name());

                    dataset.write(attr->data(), attr_type);

                    std::cout << "Name:      " << attr->name() << std::endl;
                    std::cout << "Long name: " << attr->long_name() << std::endl;
                    std::cout << "Units:     " << attr->unit() << std::endl;
                }
            }
            std::cout << std::endl;

            return *this;
        }

        template <class ParticleContainer>
        ParticleStream<ParticleContainer>& ParticleStream<ParticleContainer>::operator>>(
            ParticleContainer& /*obj*/) {
            return *this;
        }

    }  // namespace hdf5

}  // namespace ippl
