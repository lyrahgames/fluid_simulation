#ifndef __COLMAP_H__
#define __COLMAP_H__

#include <util.h>
// #include <xmath/vec.h>


// template <class T>
// struct color_rgb{
// 	T r,g,b;
// };

using color_rgbf = xmath::vec<float,3>;
using color_rgbaf = xmath::vec<float,4>;


struct colormap{
	
	static constexpr uint _max_size_ = 16;


	struct base{
		float pos;
		color_rgbf color;
	};


	public:
		/*constexpr*/ colormap(): _size{0}, _data{}, seg_idx_cache(1), bg_color{0.1,0.1,0.1}{}

		static /*constexpr*/ uint max_size(){return _max_size_;}

		/*constexpr*/ const base& operator[](uint idx) const{return _data[idx];}
		/*constexpr*/ uint size() const{return _size;}
		/*constexpr*/ const base* data() const{return _data;}

		color_rgbf operator()(float x) const{
			if (x <= _data[0].pos || x >= _data[size()-1].pos)
				return bg_color;

			const uint seg_idx = compute_seg_idx(x);

			return compute_interp(seg_idx-1, seg_idx, compute_weight(_data[seg_idx-1].pos, _data[seg_idx].pos, x));

			// if (seg_idx == 0){
			// 	return _data[0].color;
			// }else{
			// 	if (seg_idx == size()){
			// 		return _data[size()-1].color;
			// 	}else{
			// 		return compute_interp(seg_idx-1, seg_idx, compute_weight(_data[seg_idx-1].pos, _data[seg_idx].pos, x));
			// 	}
			// }
		}

		void add_base(const base& in_base){
			if (size() >= max_size())
				return;

			const uint seg_idx = compute_seg_idx_save(in_base.pos);

			const base* tmp = _data + seg_idx;
			memmove((base*)(tmp + 1), tmp, (size()-seg_idx)*sizeof(base));

			_data[seg_idx] = in_base;
			_size++;
		}


	private:
		base _data[_max_size_];
		uint _size;

		uint seg_idx_cache;

	public:
		color_rgbf bg_color;

	private:
		uint compute_seg_idx(float x){
			if ((x >= _data[seg_idx_cache-1].pos) && (x <= _data[seg_idx_cache].pos)){
				return seg_idx_cache;
			}

			uint seg_idx = size() - 1;
			for (uint i = 1; i < size()-1; i++){
				if (x <= _data[i].pos){
					seg_idx = i;
					break;
				}
			}

			seg_idx_cache = seg_idx;

			return seg_idx;
		}

		uint compute_seg_idx_save(float x) const{
			uint seg_idx = size();
			for (uint i = 0; i < size(); i++){
				if (x <= _data[i].pos){
					seg_idx = i;
					break;
				}
			}

			return seg_idx;
		}

		color_rgbf compute_interp(uint base_idx1, uint base_idx2, float w) const{
			return (1.0f - w)*_data[base_idx1].color + w*_data[base_idx2].color;
		}

		/*constexpr*/ float compute_weight(float x1, float x2, float x) const{
			return (x-x1)/(x2-x1);
		}
};


#endif // __COLMAP_H__